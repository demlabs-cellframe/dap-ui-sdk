#include <QtDebug>
#include <QProcess>
#include <QFile>
#include <QSettings>
#include <QHostAddress>

#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sys_domain.h>
#include <sys/ioctl.h> // ioctl
#include <sys/kern_control.h> // struct socketaddr_ctl
#include <net/if_utun.h> // UTUN_CONTROL_NAME

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "DapTunMac.h"
#include "DapTunWorkerMac.h"
#include "SigUnixHandler.h"
#include "DapNetworkMonitor.h"

/**
 * @brief DapTunMac::DapTunMac
 */
DapTunMac::DapTunMac()
{
    qDebug() << "DapTunMac()";
    tunWorker = (tunWorkerMac = new DapTunWorkerMac(this));
    breaker0 = tunWorkerMac->breaker(0);
    breaker1 = tunWorkerMac->breaker(1);
    initWorker();

    connect(SigUnixHandler::getInstance(), &SigUnixHandler::sigKill,
            this, &DapTunMac::tunDeviceDestroy, Qt::DirectConnection);
}

/**
 * @brief DapTunMac::tunDeviceCreate
 */
void DapTunMac::tunDeviceCreate()
{
    if (m_tunSocket > 0) {
        qInfo() << "Already created";
        return;
    }
    qDebug() << "tunDeviceCreate()";
    setTunDeviceName("tun5");

    m_tunSocket = ::open (("/dev/"+m_tunDeviceName).toLatin1().constData(),O_RDWR);

    if(m_tunSocket <= 0){
        qCritical() << "tunDeviceCreate() can't open "<< m_tunDeviceName;
        return;
    }

    if (::fcntl(m_tunSocket, F_SETFL, O_NONBLOCK) < 0) {    ;
        qCritical() << "tunDeviceCreate() can't switch socket to non-block mode";
        return;
    }

    if (::fcntl(m_tunSocket, F_SETFD, FD_CLOEXEC) < 0){    ;
        qCritical() << "tunDeviceCreate() can't switch socket to not be passed across execs";
        return;
    }
}

/**
 * @brief DapTunMac::onWorkerStarted
 */
void DapTunMac::onWorkerStarted()
{
    QProcess process;
    process.start("bash",QStringList() << "-c" << "netstat -rn|grep default| head -n 1| awk '{print $2;}'");
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

        QString run = QString("route add -host %2 %1; route delete default %1")
            .arg(m_defaultGwOld).arg(upstreamAddress()).toLatin1().constData();
        qDebug() << "cmd run [" << run << ']';
         ::system(run.toLatin1().constData() );
        run = QString("route add default %1").arg(gw());
        ::system(run.toLatin1().constData() );
    }

    DapNetworkMonitor::instance()->sltSetDefaultGateway(m_defaultGwOld);

    QString run;
    run = QString("route add -host %2 %1")
        .arg(m_defaultGwOld).arg(upstreamAddress()).toLatin1().constData();
    qDebug() << "cmd run [" << run << ']';
     ::system(run.toLatin1().constData() );

    run = QString("route delete default %1")
        .arg(m_defaultGwOld);
    qDebug() << "cmd run [" << run << ']';
     ::system(run.toLatin1().constData() );

    run = QString("ifconfig %1 %2 %3 up")
                         .arg(tunDeviceName()).arg(addr())
                        .arg(gw());
   qDebug() << "cmd run [" << run << "]" << m_addr << "],[" << m_gw << "],[" << m_sUpstreamAddress << "]";

   ::system(run.toLatin1().constData() );

   run = QString("route add default %1 -hopcount -1 -lock").arg(gw());
   qDebug() << "cmd run [" << run << ']';
     ::system(run.toLatin1().constData() );

   backupAndApplyDNS();

    qInfo() << "Created "<<m_tunDeviceName<<" network interface";
/* this is the special file descriptor that the caller will use to talk
* with the virtual interface */

    m_isCreated = true;
    emit created();
}

/**
 * @brief DapTunMac::tunDeviceDestroy
 */
void DapTunMac::tunDeviceDestroy()
{
    qDebug() << "tunDeviceDestroy() Start";

    if (!m_isCreated) // nothing created, nothing to destroy.
        return;

    ::system(QString("route delete -host %2 %1; route delete default %3; route add default %1")
            .arg(m_defaultGwOld).arg(m_addr).arg(m_gw).toLatin1().constData()  );
    ::system(QString("ifconfig %1 down")
             .arg(tunDeviceName()).toLatin1().constData() );

    ::close(m_tunSocket);
    qDebug() << "tunDeviceDestroy() Closed tun socket";

    qInfo() << "Close tun device (and usualy destroy that after)";
    m_tunSocket = -1;

    getBackDNS();

    emit destroyed();
}

DapTunMac::~DapTunMac() {
}

void DapTunMac::workerStop()
{
    if ( ::write( breaker1, "\0", 1) <= 0) {
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
    onWorkerStopped();
}

void DapTunMac::workerPause() {
    if ( ::write( breaker1, "\0", 1) <= 0) {
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
}

void DapTunMac::signalWriteQueueProc()
{
    if ( ::write( breaker1, "w", 1) <= 0) {
        qCritical() <<"Can't write to the breaker's pipe!";
    }
}

void DapTunMac::workerPrepare()
{
    qInfo() <<"Prepare worker before the work";
}

void DapTunMac::backupAndApplyDNS()
{
    qDebug() << "backupAndApplyDNS()";
    QSettings dnsSettigs;
    dnsSettigs.setValue("ActiveNetIfs", "");

    QFile file(":/macos/GetActiveNetIFs.sh");
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning() << "Can't get resource script \":/macos/GetActiveNetIFs.sh\". Your DNS settings could be wrong";
        return;
    }
    QTextStream in(&file);
    QString script = in.readAll();

    QProcess process;
    process.setProgram("/bin/bash");
    process.start();
    process.write(script.toLatin1().constData());
    process.closeWriteChannel();
    process.waitForFinished();

    QString l_strActiveNetIfs = process.readAllStandardOutput();
    dnsSettigs.setValue("ActiveNetIfs", l_strActiveNetIfs);

    QStringList l_lActiveNetIfs = l_strActiveNetIfs.split('\n');

    bool isAnyNetIfs = false;
    for (QString netIf : l_lActiveNetIfs){
        if (netIf.isEmpty())
            continue;
        isAnyNetIfs = true;

        process.setProgram("/bin/bash");
        process.start();
        process.write(QString("networksetup -getdnsservers %1 \n").arg(netIf).toLatin1().constData());
        process.closeWriteChannel();
        process.waitForFinished();
        QString l_rawDNS = process.readAllStandardOutput();

        QStringList l_listOfDNS = l_rawDNS.split('\n');
        QString l_finalDNSString;
        for (auto l_dns : l_listOfDNS){ // to prevent error from script
            QHostAddress address(l_dns);
            if (QAbstractSocket::IPv4Protocol == address.protocol()){
                //qDebug() << "Valid IPv4 address: " << l_dns;
                l_finalDNSString += l_dns + " ";
            }
            else if (QAbstractSocket::IPv6Protocol == address.protocol()){
                //qDebug() << "Valid IPv6 address: " << l_dns;
            }
            else{
                //qDebug() << "Unknown or invalid address: " << l_dns;
            }
        }
        if (!l_finalDNSString.isEmpty())
            dnsSettigs.setValue(netIf, l_finalDNSString);
        else
            dnsSettigs.setValue(netIf, "empty");//m_defaultGwOld);

        //Logs for dns for particular netif
        //QStringList l_lDNS = a_strDNS.split('\n');
        //qDebug() << "----- DNS of " << netIf << ": ";
        //for (QString l_dns : l_lDNS)
        //    if (!l_dns.isEmpty())
        //        qDebug() << "------------------ " << l_dns;

        QString run = QString("networksetup -setdnsservers %1 %2").
                             arg(netIf).arg(gw());
        qDebug() << "cmd run [" << run << ']';
        ::system(run.toLatin1().constData() );
    }

    if (!isAnyNetIfs){
        qWarning() << "Can't get any active NetIf. Your DNS configuration could be wrong.";
        return;
    }

    //Flush dns cache
    QString run = QString("sudo killall -HUP mDNSResponder");
    qDebug() << "cmd run [" << run << ']';
    ::system(run.toLatin1().constData() );

    dnsSettigs.sync();
}

void DapTunMac::getBackDNS()
{
    qDebug() << "getBackDNS()";
    QSettings dnsSettigs;
    QString l_strActiveNetIfs = dnsSettigs.value("ActiveNetIfs").toString();

    QStringList l_lActiveNetIfs = l_strActiveNetIfs.split('\n');

    for (QString netIf : l_lActiveNetIfs){
        if (netIf.isEmpty())
            continue;

        QString l_oldDNS = dnsSettigs.value(netIf).toString();
        l_oldDNS.replace("\n", " ");
        QString run = QString("networksetup -setdnsservers %1 %2").
                             arg(netIf).arg(l_oldDNS);
        qDebug() << "cmd run [" << run << ']';
        ::system(run.toLatin1().constData() );
    }

    //Flush dns cache again, after restoring dns settings
    QString run = QString("sudo killall -HUP mDNSResponder");
    qDebug() << "cmd run [" << run << ']';
    ::system(run.toLatin1().constData() );
}
