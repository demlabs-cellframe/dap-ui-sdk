#include "DapTunAndroid.h"


#include <QtAndroid>
#include <QtAndroidExtras>
#include <jni.h>
#include <unistd.h>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <sys/stat.h>
#include <sys/socket.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include "DapSession.h"
#include "DapDNSController.h"

DapTunAndroid::DapTunAndroid()
    : m_dnsController(new DapDNSController(this))
{
    qDebug() << "[DapTunAndroid::DapTunAndroid]";
    tunWorker = new DapTunWorkerUnix(this);
    tunWorkerAndroid = dynamic_cast<DapTunWorkerUnix*>(tunWorker);
    breaker0 = tunWorkerAndroid->breaker(0);
    breaker1 = tunWorkerAndroid->breaker(1);
    initWorker();
    
    // Connect DNS controller signals for logging
    connect(m_dnsController, &DapDNSController::errorOccurred, this, [this](const QString &error) {
        qWarning() << "[TUN] DNS Controller error:" << error;
    });
    
    connect(m_dnsController, &DapDNSController::dnsServersChanged, this, [this](const QStringList &servers) {
        qInfo() << "[TUN] DNS servers updated to:" << servers;
    });
    
    connect(m_dnsController, &DapDNSController::dnsRestored, this, []() {
        qInfo() << "[TUN] DNS settings restored to original configuration";
    });
}


void DapTunAndroid::tunDeviceCreate()
{
    emit nativeCreateRequest();
}

void DapTunAndroid::tunDeviceDestroy()
{
    QtAndroid::androidService().callMethod<void>("stop" DAP_BRAND "ServiceNative", "()V");
    qInfo() << "Close tun device (and usualy destroy that after)";
    
    // Restore original DNS settings
    if (m_dnsController && m_dnsController->isDNSSet()) {
        qInfo() << "[TUN] Restoring original DNS settings";
        if (!m_dnsController->restoreDefaultDNS()) {
            qWarning() << "[TUN] Failed to restore original DNS settings";
        }
    }
    
    if(m_tunSocket>0){
        ::close(m_tunSocket);
    }
    m_tunSocket = -1;
    emit destroyed();
}

void DapTunAndroid::onWorkerStarted() {
    DapTunAbstract::onWorkerStarted();
    
    // Set DNS to VPN server if gateway is available
    if (!gw().isEmpty()) {
        qInfo() << "[TUN] Setting VPN DNS server:" << gw();
        if (!m_dnsController->setDNSServers(QStringList{gw()})) {
            qWarning() << "[TUN] Failed to set DNS servers for VPN connection";
        }
    }
    
    m_isCreated = true;
    emit created();
}

void DapTunAndroid::workerStart() {
    if (m_tunSocket <= 0) {
        qCritical() << "Invalid socket!";
        return;
    }
    tunWorker->setTunSocket(m_tunSocket);
    tunFuture = QtConcurrent::run(tunWorkerAndroid, &DapTunWorkerUnix::loop);
    onWorkerStarted();
}

void DapTunAndroid::workerStop()
{
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
    tunFuture.waitForFinished();
    onWorkerStopped();
}

void DapTunAndroid::workerPause()
{
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
    tunFuture.waitForFinished();
    onWorkerStopped(); // TODO: remove it when I find out how to allow requests bypass the socket
}

void DapTunAndroid::signalWriteQueueProc()
{
    if ( ::write( breaker1, "w", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
    }
}

void DapTunAndroid::workerPrepare()
{
    qInfo() <<"Prepare worker before the work";
}

void DapTunAndroid::addNewUpstreamRoute(const QString &a_dest) {
    Q_UNUSED(a_dest);
}
