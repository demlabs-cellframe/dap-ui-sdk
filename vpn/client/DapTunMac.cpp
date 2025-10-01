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
    qDebug() << "DapTunMac::tunDeviceCreate - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles UTUN creation automatically
    qCritical() << "Direct UTUN creation is deprecated. Use NetworkExtension instead.";
    return;
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
    qDebug() << "DapTunMac::backupAndApplyDNS - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles DNS configuration automatically through NEDNSSettings
    qWarning() << "Direct DNS configuration is deprecated. Use NetworkExtension instead.";
    return;
}

void DapTunMac::getBackDNS()
{
    qDebug() << "DapTunMac::getBackDNS - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles DNS restoration automatically
    qWarning() << "Direct DNS restoration is deprecated. Use NetworkExtension instead.";
    return;
}

void DapTunMac::addNewUpstreamRoute(const QString &a_dest) {
    qDebug() << "DapTunMac::addNewUpstreamRoute - DEPRECATED: Using NetworkExtension instead";
    
    // This method is deprecated in favor of NetworkExtension
    // NetworkExtension handles routing automatically through NEPacketTunnelNetworkSettings
    qWarning() << "Direct route management is deprecated. Use NetworkExtension instead.";
    return;
}
