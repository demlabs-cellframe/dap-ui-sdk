#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QSettings>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <arpa/inet.h>

#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "DapTunLinux.h"
#include "DapTunWorkerUnix.h"
#include "DapNetworkMonitor.h"
#include "SigUnixHandler.h"

/**
 * @brief DapTunLinux::DapTunLinux
 */
DapTunLinux::DapTunLinux()
{
    if(nmcliVersion.size()==0){ // If not detected before - detect nmcli version
        QProcess cmdProcess;
        // Thats command takes the last one piece of 'nmcli -v' output
        //        cmdProcess.start("/bin/sh  'for i in `nmcli -v`; do a=$i; done; echo $a'");
        cmdProcess.start("nmcli -v");
        cmdProcess.waitForFinished(-1);
        QByteArray cmdOutput = cmdProcess.readAllStandardOutput();
        nmcliVersion= QString::fromUtf8(cmdOutput).split(' ').takeLast();
        nmcliVersion=nmcliVersion.replace("\n","");
        QStringList nmcliVersionNumbers= nmcliVersion.split('.');
        if(nmcliVersionNumbers.size() ==3) {
            qDebug()<< QString("nmcli version detected: %1.%2.%3")
                       .arg(nmcliVersionNumbers.at(0).toInt())
                       .arg(nmcliVersionNumbers.at(1).toInt())
                       .arg(nmcliVersionNumbers.at(2).toInt());
        } else {
            qFatal("nmcli client not found");
        }
    }
    connect(SigUnixHandler::getInstance(), &SigUnixHandler::sigKill,
            this, &DapTunLinux::tunDeviceDestroy, Qt::DirectConnection);
}

/**
 * @brief DapTunLinux::requestTunDeviceCreate
 */
void DapTunLinux::tunDeviceCreate()
{
    qDebug() << "[DapTunLinux::tunDeviceCreate]";
    if (m_tunSocket > 0) {
        qInfo() << "Socket already open";
        return;
    }
    int fd=-1;
    struct ifreq ifr;
    char clonedev[] = "/dev/net/tun";
    char dev[IFNAMSIZ] = {0};
    int flags = IFF_TUN |IFF_NO_PI;

    /* Arguments taken by the function:
    *
    * char *dev: the name of an interface (or '\0'). MUST have enough
    *   space to hold the interface name if '\0' is passed
    * int flags: interface flags (eg, IFF_TUN etc.)
    */

    /* open the clone device */
    if ((fd = ::open(clonedev, O_RDWR)) < 0 ) {
        qCritical() << "Can't open /dev/net/tun device!";
        return;
    }

    ::memset(&ifr,0,sizeof(ifr));

    ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

    if (dev[0]) {
        /* if a device name was specified, put it in the structure; otherwise,
      * the kernel will try to allocate the "next" device of the
      * specified type */
        ::strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }else
        /* try to create the device */


        if (::ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
            qCritical() << "Can't create tun network interface!";

            ::close(fd);
            return;
        }

    /* if the operation was successful, write back the name of the
    * interface to the variable "dev", so the caller can know
    * it. Note that the caller MUST reserve space in *dev (see calling
    * code below) */

    ::strncpy(dev, ifr.ifr_name,IFNAMSIZ);
    m_tunDeviceName = QString::fromLatin1(dev);
    qInfo() << "Created "<<m_tunDeviceName<<" network interface";

    /* this is the special file descriptor that the caller will use to talk
    * with the virtual interface */
    m_tunSocket = fd;
}

QString DapTunLinux::runBashCmd(const QString& cmd)
{
    QProcess process;
    process.start("bash", QStringList() << "-c" << cmd);
    process.waitForFinished(-1);
    QString result = process.readAllStandardOutput();

    if(!result.isEmpty())
        result.chop(1); // delete \n symbol
    else
        qWarning() << "Result bash cmd " << cmd << "is empty";

    return result;
}

void DapTunLinux::saveCurrentConnectionInterfaceData()
{
    // nmcli -t -f NAME,TIMESTAMP con show | sort -t: -nk2 | tail -n1 | cut -d: -f1
    QString result = runBashCmd("nmcli -terse --fields NAME,DEVICE con show | head -n1");

    QStringList res = result.split(":");

    if(res.length() != 2) {
        qWarning() << "Can't get current connection interface name!";
        return;
    }

    m_lastUsedConnectionName = res[0];
    m_lastUsedConnectionDevice = res[1];

    qDebug() << "DeviceName name:" << m_lastUsedConnectionDevice
             << "Interface Name:" << m_lastUsedConnectionName;

}

void DapTunLinux::disableIPV6()
{
    static QString disableTmplCmd = "echo 1 > /proc/sys/net/ipv6/conf/%1/disable_ipv6";
    if(!m_lastUsedConnectionDevice.isEmpty()) {
        ::system(disableTmplCmd.arg(m_lastUsedConnectionDevice).toLatin1().data());
        return;
    }

    qWarning() << "Can't disable IPV6 m_lastUsedConnectionDevice is empty";
}

void DapTunLinux::enableIPV6()
{
    static QString enableTmplCmd = "echo 0 > /proc/sys/net/ipv6/conf/%1/disable_ipv6";
    if(!m_lastUsedConnectionDevice.isEmpty()) {
        ::system(enableTmplCmd.arg(m_lastUsedConnectionDevice).toLatin1().data());
        return;
    }
    qWarning() << "Can't enable IPV6 m_lastUsedConnectionDevice is empty";
}

/**
 * @brief DapTunLinux::checkDefaultGetaweyMetric
 */
void DapTunLinux::checkDefaultGetaweyMetric()
{
    enum {DESTINATION, GATEWAY, GENMASK, FLAGS,
          METRIC, REF, USE, IFACE};
    QStringList result = runBashCmd("route | grep default").simplified().split(" ");

    if(result.isEmpty()) {
        qWarning() << "Can't get current connection interface name!";
    } else if(result.length() != 8) {
        qWarning() << "Something wrong with result:" << result;
    } else if(result[METRIC] == "0") {
        QProcess process;
        process.start("bash", QStringList() << "-c" << QString("ifmetric %1 15").arg(result[IFACE]));
        process.waitForFinished(-1);
        qInfo() << "Metric " << result[IFACE] << "change to 15";
    }
}


/* Add new data to rtattr */
int rtattr_add(struct nlmsghdr *n, unsigned int maxlen, int type, const void *data, int alen)
{
    int len = RTA_LENGTH(alen);
    struct rtattr *rta;

    if (NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len) > maxlen) {
        fprintf(stderr, "rtattr_add error: message exceeded bound of %d\n", maxlen);
        return -1;
    }

    rta = ((struct rtattr *) (((void *) (n))) + NLMSG_ALIGN(n->nlmsg_len));
    rta->rta_type = type;
    rta->rta_len = len;

    if (alen) {
        memcpy(RTA_DATA(rta), data, alen);
    }

    n->nlmsg_len = NLMSG_ALIGN(n->nlmsg_len) + RTA_ALIGN(len);

    return 0;
}


int sSetRoute(in_addr_t host_addr, in_addr_t gw_addr)
{
    int sock_r = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_r < 0){
        qCritical()<< "Socket opening error.";
        return sock_r;
    }

    struct {
        struct nlmsghdr n;
        struct rtmsg r;
        char buf[4096];
    } nl_request;


    nl_request.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    nl_request.r.rtm_table = RT_TABLE_MAIN;
    nl_request.r.rtm_scope = RT_SCOPE_NOWHERE;
    nl_request.n.nlmsg_flags = 0;

    nl_request.n.nlmsg_type = RTM_NEWROUTE;
    nl_request.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_REPLACE;
    nl_request.r.rtm_type = RTN_UNICAST;
    nl_request.r.rtm_protocol = RTPROT_KERNEL;

    nl_request.r.rtm_family = AF_INET;
    nl_request.r.rtm_scope = RT_SCOPE_LINK;

    nl_request.r.rtm_family = AF_INET;
    nl_request.r.rtm_dst_len = 32;

    rtattr_add(&nl_request.n, sizeof(nl_request), RTA_DST, &host_addr, sizeof(host_addr));
    nl_request.r.rtm_scope = 0;
    nl_request.r.rtm_family =AF_INET;

    rtattr_add(&nl_request.n, sizeof(nl_request), RTA_GATEWAY, &gw_addr, sizeof(gw_addr));

    int if_idx = if_nametoindex("enp5s0");
    rtattr_add(&nl_request.n, sizeof(nl_request), RTA_OIF, &if_idx, sizeof(int));


    int rc = send(sock_r, &nl_request, sizeof(nl_request), 0);
    close( sock_r );

    return rc;

//    int sock_r = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP );
//    if (sock_r < 0){
//        qCritical()<< "Socket opening error.";
//        return -1;
//    }

//    struct rtentry route;
//    memset(&route, 0, sizeof(struct rtentry));

//    struct sockaddr_in *addr = (struct sockaddr_in *)&route.rt_gateway;
//    addr->sin_family = AF_INET;
//    addr->sin_addr.s_addr = gw_addr;

//    addr = (struct sockaddr_in*) &route.rt_dst;
//    addr->sin_family = AF_INET;
//    addr->sin_addr.s_addr = host_addr;

//    addr = (struct sockaddr_in*) &route.rt_genmask;
//    addr->sin_family = AF_INET;
//    addr->sin_addr.s_addr = inet_addr( "255.255.255.255" );

//    route.rt_flags = RTF_UP | RTF_GATEWAY;
//    route.rt_metric = 0;

//    int rc = ioctl( sock_r, SIOCADDRT, &route );
//    close( sock_r );

//    return rc;

}

/**
 * @brief DapTunLinux::onWorkerStarted
 */
void DapTunLinux::onWorkerStarted()
{
    qDebug() << "tunnelCreate()";
    QProcess process;

    if(m_tunSocket <=0){
        qCritical()<< "Can't bring up network interface ";
        return;
    }

    checkDefaultGetaweyMetric();
    saveCurrentConnectionInterfaceData();
    disableIPV6();
    process.start("bash", QStringList() << "-c" <<  "netstat -rn|grep 'UG '| head -n 1| awk '{print $2;}'");
    process.waitForFinished(-1);
    m_defaultGwOld=process.readAllStandardOutput();
    m_defaultGwOld.chop(1);
    if(m_defaultGwOld.isEmpty()){
        qWarning() << "There is no default gateway, may be we've broken that last time? Trying to check that...";
        process.start("bash",QStringList() << "-c" << QString("netstat -rn|grep %1|awk '{print $2;}'").arg(upstreamAddress())  );
        process.waitForFinished(-1);

        m_defaultGwOld=process.readAllStandardOutput();
        m_defaultGwOld.chop(1);
        if(m_defaultGwOld.isEmpty()){
            qWarning() << "Not found old gateway, looks like its better to restart the network";
            return;
        }

        if (sSetRoute(inet_addr( upstreamAddress().toLocal8Bit().data()), inet_addr( m_defaultGwOld.toLocal8Bit().data() )) < 0){
            qWarning() << "Routing error.";
            return;
        }
    }

    DapNetworkMonitor::instance()->sltSetDefaultGateway(m_defaultGwOld);

    QString run = QString("ip route del default via %1").arg(m_defaultGwOld);
    qDebug() << "cmd run [" << run << ']';
     ::system(run.toLatin1().constData() );

    ::system("nmcli c delete " DAP_BRAND);

    if(!isLocalAddress(upstreamAddress()))
    {
        if (sSetRoute(inet_addr( upstreamAddress().toLocal8Bit().data() ), inet_addr( m_defaultGwOld.toLocal8Bit().data() )) < 0){
            qWarning() << "Routing error.";
            return;
        }
    }

    QString cmdConnAdd = QString(
                "nmcli connection add type tun con-name " DAP_BRAND " autoconnect false ifname %1 "
                "mode tun ip4 %2 gw4 %3")
            .arg(tunDeviceName()).arg(addr()).arg(gw());

    qDebug() << "[Cmd to created interface: " <<  cmdConnAdd.toLatin1().constData();

    ::system(cmdConnAdd.toLatin1().constData());

    ::system("nmcli connection modify " DAP_BRAND
             " +ipv4.ignore-auto-routes true");

    ::system("nmcli connection modify " DAP_BRAND
             " +ipv4.ignore-auto-dns true");

    ::system((QString("nmcli connection modify " DAP_BRAND
        " +ipv4.dns-search " DAP_BRAND)
        ).toLatin1().constData());

    ::system("nmcli connection modify " DAP_BRAND " ipv4.dns-priority 10");

    ::system("nmcli connection modify " DAP_BRAND
             " +ipv4.method manual");

    ::system((QString("nmcli connection modify %1 +ipv4.dns %2")
              .arg(DAP_BRAND).arg(gw())).toLatin1().constData());

    ::system("nmcli connection modify " DAP_BRAND
             " +ipv4.route-metric 10");

    ::system("nmcli connection up " DAP_BRAND);
    m_isCreated = true;
    emit created();
}

/**
 * @brief DapTunLinux::onWorkerStopped
 */
void DapTunLinux::tunDeviceDestroy()
{
    ::system(QString("ifconfig %1 down")
             .arg(tunDeviceName()).toLatin1().constData());
    ::system("nmcli connection down " DAP_BRAND);
    ::system("nmcli connection delete " DAP_BRAND);

    QString run = QString("ip route add default via %1").arg(m_defaultGwOld);
        qDebug() << "cmd run [" << run << ']';
         ::system(run.toLatin1().constData() );

    enableIPV6();

    ::system(QString("nmcli connection up \"%1\"")
             .arg(m_lastUsedConnectionName).toLatin1().constData());

    DapTunUnixAbstract::tunDeviceDestroy();
}

