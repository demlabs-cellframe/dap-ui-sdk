#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QSettings>
#include <QHostAddress>
#include <QThread>
#include <QTimer>

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
#include "DapDNSController.h"

/**
 * @brief DapTunMac::DapTunMac
 */
DapTunMac::DapTunMac()
    : DapTunAbstract()
    , m_dnsController(new DapDNSController(this))
{
    qDebug() << "DapTunMac()";
    tunThread = new QThread();
    tunWorker = new DapTunWorkerMac(this);
    connect(this, &DapTunAbstract::sigStartWorkerLoop, tunWorker, &DapTunWorkerAbstract::loop);
    breaker0 = tunWorker->breaker(0);
    breaker1 = tunWorker->breaker(1);
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

    QString run;
    run = QString("route add default %1 -hopcount -1 -lock").arg(gw());
    qDebug() << "cmd run [" << run << ']';
    ::system(run.toLatin1().constData());

    // Setting up DNS via DapDNSController
    if (!m_dnsController->setDNSServers(QStringList{m_gw})) {
        qWarning() << "Failed to set DNS servers";
    } else {
        // Start DNS monitoring to protect against manual changes
        qInfo() << "[DapTunMac] Starting DNS monitoring for VPN connection";
        m_dnsController->setExpectedDNSServers(QStringList{m_gw});
        m_dnsController->setVPNConnectionState(true);
        m_dnsController->startDNSMonitoring(5000); // Check every 5 seconds
    }

    qInfo() << "Created "<<m_tunDeviceName<<" network interface";
    m_isCreated = true;
    emit created();
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

   // Setting up DNS via DapDNSController instead of old methods
   if (!m_dnsController->setDNSServers(QStringList{m_gw})) {
       qWarning() << "Failed to set DNS servers via DapDNSController";
   } else {
       // Start DNS monitoring to protect against manual changes
       qInfo() << "[DapTunMac] Starting DNS monitoring for VPN connection in onWorkerStarted";
       m_dnsController->setExpectedDNSServers(QStringList{m_gw});
       m_dnsController->setVPNConnectionState(true);
       m_dnsController->startDNSMonitoring(5000); // Check every 5 seconds
   }

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

    // Stop DNS monitoring and restore original DNS settings via DapDNSController
    qInfo() << "[DapTunMac] Stopping DNS monitoring and restoring original DNS settings";
    m_dnsController->setVPNConnectionState(false);
    m_dnsController->stopDNSMonitoring();
    
    if (!m_dnsController->restoreDefaultDNS()) {
        qWarning() << "Failed to restore original DNS settings via DapDNSController";
    }

    qInfo() << "Close tun device (and usualy destroy that after)";
    m_tunSocket = -1;

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

void DapTunMac::addNewUpstreamRoute(const QString &a_dest) {
    QString run;
    run = QString("route add -host %2 %1")
        .arg(m_defaultGwOld).arg(qPrintable(a_dest));
    qDebug() << "cmd run [" << run << ']';
     ::system(run.toLatin1().constData() );
}
