#include <QtDebug>
#include <QProcess>
#include <QFile>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

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
if(nmcliVersion.size()==0) // If not detected before - detect nmcli version
{
        QProcess cmdProcess;
        // Command to get nmcli version (program + args, not a single string)
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LC_ALL", "C");
        env.insert("LANG", "C");
        cmdProcess.setProcessEnvironment(env);
        cmdProcess.start("nmcli", QStringList() << "-v");
if(!cmdProcess.waitForFinished(3000))
{
            qWarning() << "nmcli version request timeout";
        }
        QByteArray cmdOutput = cmdProcess.readAllStandardOutput();
if(cmdOutput.isEmpty())
{
            // some distros print to stderr
            cmdOutput = cmdProcess.readAllStandardError();
        }
        // Robust parse: find last token with digits and dots
        QString output = QString::fromUtf8(cmdOutput).trimmed();
        QStringList tokens = output.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        QString candidate;
        for(int i=tokens.size()-1;i>=0;--i){
            if(tokens.at(i).contains(QRegularExpression("^[0-9]+(\\.[0-9]+)*$"))){
                candidate = tokens.at(i);
                break;
            }
        }
        nmcliVersion = candidate;
        nmcliVersion=nmcliVersion.replace("\n","");
        QStringList nmcliVersionNumbers= nmcliVersion.split('.');
        if(nmcliVersionNumbers.size() >= 2) {
            qDebug()<< QString("nmcli version detected: %1.%2.%3")
                            .arg(nmcliVersionNumbers.at(0).toInt())
                            .arg(nmcliVersionNumbers.at(1).toInt())
                            .arg(nmcliVersionNumbers.size()>2 ? nmcliVersionNumbers.at(2).toInt() : 0);
        }
        else
        {
            qWarning() << QString("nmcli client not found or unparsable output: \"%1\"").arg(output);
            Q_ASSERT_X(false, "DapTunLinux", "nmcli client not found or unparsable output");
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
    if(m_tunSocket > 0)
    {
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
    if((fd = ::open(clonedev, O_RDWR)) < 0 )
    {
        qCritical() << "Can't open /dev/net/tun device!";
        return;
    }
    
    ::memset(&ifr,0,sizeof(ifr));
    ifr.ifr_flags = flags;   /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */
    
    if(dev[0])
    {
        ::strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }
    else
        /* try to create the device */
        
        
        if(::ioctl(fd, TUNSETIFF, (void *) &ifr) < 0)
        {
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
    QString result = process.readAllStandardOutput().trimmed();

    if(result.isEmpty())
        qWarning() << "Result of bash cmd " << cmd << " is empty";

    return result;
}

void DapTunLinux::saveCurrentConnectionInterfaceData()
{
    QString result = runBashCmd("nmcli -t --fields NAME,DEVICE connection show --active | head -n1");
    QStringList res = result.split(":");

    if(res.length() != 2) {
        qWarning() << "Can't get current connection interface name!";
        return;
    }

    m_lastUsedConnectionName = res[0];
    m_lastUsedConnectionDevice = res[1];

    qDebug() << "DeviceName:" << m_lastUsedConnectionDevice
             << "Interface Name:" << m_lastUsedConnectionName;
}

void DapTunLinux::disableIPV6()
{
    static QString disableTmplCmd = "echo 1 > /proc/sys/net/ipv6/conf/%1/disable_ipv6";
    if(!m_lastUsedConnectionDevice.isEmpty()) {
        ::system(disableTmplCmd.arg(m_lastUsedConnectionDevice).toLatin1().data());
        return;
    }
    qWarning() << "Can't disable IPV6, m_lastUsedConnectionDevice is empty";
}

void DapTunLinux::enableIPV6()
{
    static QString enableTmplCmd = "echo 0 > /proc/sys/net/ipv6/conf/%1/disable_ipv6";
    if(!m_lastUsedConnectionDevice.isEmpty()) {
        ::system(enableTmplCmd.arg(m_lastUsedConnectionDevice).toLatin1().data());
        return;
    }
    qWarning() << "Can't enable IPV6, m_lastUsedConnectionDevice is empty";
}

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
        qInfo() << "Metric for " << result[IFACE] << "changed to 15";
    }
}

bool DapTunLinux::connectionExists(const QString &connName)
{
    QString result = runBashCmd(QString("nmcli -t --fields NAME connection show | grep -w '%1'").arg(connName));
    return !result.isEmpty();
}

/**
 * @brief DapTunLinux::onWorkerStarted
 */
void DapTunLinux::onWorkerStarted()
{
    qDebug() << "tunnelCreate()";
    QProcess process;

    if(m_tunSocket <= 0){
        qCritical() << "Can't bring up network interface";
        return;
    }

    bool updateRouteTable = ( upstreamSocket() != -1);// We expect that its could be only no-route situation
    if( updateRouteTable ){
        checkDefaultGetaweyMetric();
        saveCurrentConnectionInterfaceData();
        disableIPV6();

        process.start("bash", QStringList() << "-c" <<  "netstat -rn | grep 'UG ' | head -n 1 | awk '{print $2;}'");
        process.waitForFinished(-1);
        m_defaultGwOld = process.readAllStandardOutput().trimmed();

        if(m_defaultGwOld.isEmpty()){
            qWarning() << "There is no default gateway, maybe we broke it last time? Trying to check that...";
            process.start("bash", QStringList() << "-c" << QString("netstat -rn | grep %1 | awk '{print $2;}'").arg(upstreamAddress()));
            process.waitForFinished(-1);
            m_defaultGwOld = process.readAllStandardOutput().trimmed();

            if(m_defaultGwOld.isEmpty()){
                qWarning() << "Not found old gateway, looks like it's better to restart the network";
                return;
            }

            QString run = QString("route add -host %2 gw %1")
                    .arg(m_defaultGwOld).arg(upstreamAddress()).toLatin1().constData();
            ::system(run.toLatin1().constData() );
        }

        // Add all CDBs into routing exeption
        for (const auto &str : m_routingExceptionAddrs){
            QString run = QString("route add -host %2 gw %1")
                    .arg(m_defaultGwOld).arg(str);
            ::system(run.toLatin1().constData() );
        }

        DapNetworkMonitor::instance()->sltSetDefaultGateway(m_defaultGwOld);

        QString run = QString("ip route del default via %1").arg(m_defaultGwOld);
        qDebug() << "cmd run [" << run << ']';
        ::system(run.toLatin1().constData());
    }

    qDebug() << "nmcli c delete " DAP_BRAND;
    ::system("nmcli c delete " DAP_BRAND);
    
    if( updateRouteTable ){
        if(!isLocalAddress(upstreamAddress()))
        {
            // This route dont need if address is local
            QString run = QString("route add -host %2 gw %1 metric 10")
                    .arg(m_defaultGwOld).arg(upstreamAddress()).toLatin1().constData();
            qDebug() << "Execute "<<run;
            ::system(run.toLatin1().constData());
        }
    }
    QString cmdConnAdd;
    if( updateRouteTable ){
        cmdConnAdd = QString(
                "nmcli connection add type tun con-name " DAP_BRAND " autoconnect false ifname %1 "
                "mode tun ip4 %2 gw4 %3")
            .arg(tunDeviceName()).arg(addr()).arg(gw());
    }else{
        cmdConnAdd = QString(
                "nmcli connection add type tun con-name " DAP_BRAND " autoconnect false ifname %1 "
                "mode tun ip4 %2")
            .arg(tunDeviceName()).arg(addr());
    }
    
    qDebug() << "[Cmd to created interface: " <<  cmdConnAdd.toLatin1().constData();
    
    ::system(cmdConnAdd.toLatin1().constData());

    if(!connectionExists(DAP_BRAND)) {
        qCritical() << "Failed to create connection " DAP_BRAND;
        return;
    }

    // qDebug() << "nmcli connection down " DAP_BRAND;
    // ::system(QString("nmcli connection down " DAP_BRAND).toLatin1().constData());

    // qDebug() << "nmcli connection up " DAP_BRAND;
    // ::system(QString("nmcli connection up " DAP_BRAND).toLatin1().constData());

    if( updateRouteTable ){
        qDebug() << "nmcli connection modify " DAP_BRAND " +ipv4.ignore-auto-routes true";
        ::system("nmcli connection modify " DAP_BRAND
                 " +ipv4.ignore-auto-routes true");

        qDebug() << "nmcli connection modify " DAP_BRAND " +ipv4.ignore-auto-dns true";
        ::system("nmcli connection modify " DAP_BRAND
                 " +ipv4.ignore-auto-dns true");

        QString dnsSearchCmd = QString("nmcli connection modify " DAP_BRAND " +ipv4.dns-search " DAP_BRAND).toLatin1().constData();
        qDebug() << dnsSearchCmd;
        ::system(dnsSearchCmd.toLatin1().constData());

        qDebug() << "nmcli connection modify " DAP_BRAND " ipv4.dns-priority 10";
        ::system("nmcli connection modify " DAP_BRAND " ipv4.dns-priority 10");
    }
    qDebug() << "nmcli connection modify " DAP_BRAND " +ipv4.method manual";
    ::system("nmcli connection modify " DAP_BRAND
             " +ipv4.method manual");



    if( updateRouteTable ){
        QString dnsCmd = QString("nmcli connection modify %1 +ipv4.dns %2")
                             .arg(DAP_BRAND).arg(gw()).toLatin1().constData();
        qDebug() << dnsCmd;
        ::system(dnsCmd.toLatin1().constData());

        qDebug() << "nmcli connection modify " DAP_BRAND " +ipv4.route-metric 10";
        ::system("nmcli connection modify " DAP_BRAND
                 " +ipv4.route-metric 10");
    }


    QString connUpCmd = QString("nmcli connection up %1").arg(DAP_BRAND).toLatin1().constData();
    qDebug() << connUpCmd;
    int result = ::system(connUpCmd.toLatin1().constData());
    if(result != 0) {
        qCritical() << "Failed to bring up connection " DAP_BRAND;
        return;
    }

    QString cmdRouteToGW = QString("route add -host %1 dev %2").arg(gw()).arg(tunDeviceName());
    qDebug() << "Executing cmdRouteToGW: " <<cmdRouteToGW;
    ::system(cmdRouteToGW.toLatin1());


    m_isCreated = true;
    emit created();
}

/**
 * @brief DapTunLinux::onWorkerStopped
 */
void DapTunLinux::tunDeviceDestroy()
{
    if(!connectionExists(DAP_BRAND)) {
        qWarning() << "Connection " DAP_BRAND " does not exist, skipping deletion.";
        return;
    }

    bool updateRouteTable = ( upstreamSocket() != -1);// We expect that its could be only no-route situation

    ::system(QString("ifconfig %1 down").arg(tunDeviceName()).toLatin1().constData());
    ::system("nmcli connection down " DAP_BRAND);
    ::system("nmcli connection delete " DAP_BRAND);

    if(updateRouteTable)    {
        QString run = QString("ip route add default via %1").arg(m_defaultGwOld);
        qDebug() << "cmd run [" << run << ']';
        ::system(run.toLatin1().constData());

        for (const auto &str : m_routingExceptionAddrs)
        {
            QString run = QString("route del %1")
                    .arg(str);
            ::system(run.toLatin1().constData() );
        }

        enableIPV6();

        ::system(QString("nmcli connection up \"%1\"")
                     .arg(m_lastUsedConnectionName).toLatin1().constData());
    }

    // resetNetworkSettingsToDefault();

    DapTunUnixAbstract::tunDeviceDestroy();
}

