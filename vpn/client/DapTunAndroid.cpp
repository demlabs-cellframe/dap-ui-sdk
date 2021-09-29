#include "DapTunAndroid.h"


#include <QtAndroid>
#include <QtAndroidExtras>
#include <jni.h>
#include <unistd.h>
#include <QTcpSocket>
#include <sys/stat.h>
#include <sys/socket.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#include "DapSession.h"

DapTunAndroid::DapTunAndroid()
{
    qDebug() << "[DapTunAndroid::DapTunAndroid]";
    tunWorker = (tunWorkerAndroid = new DapTunWorkerUnix(this));
    breaker0 = tunWorkerAndroid->breaker(0);
    breaker1 = tunWorkerAndroid->breaker(1);
    initWorker();
}


void DapTunAndroid::tunDeviceCreate()
{
    emit nativeCreateRequest();
}

void DapTunAndroid::tunDeviceDestroy()
{
    QtAndroid::androidService().callMethod<void>("stopKelVPNServiceNative", "()V");
    qInfo() << "Close tun device (and usualy destroy that after)";
    if(m_tunSocket>0){
        ::close(m_tunSocket);
    }
    m_tunSocket = -1;
    emit destroyed();
}

void DapTunAndroid::onWorkerStarted() {
    DapTunAbstract::onWorkerStarted();
    m_isCreated = true;
    emit created();
}

void DapTunAndroid::workerStart() {
    DapTunAbstract::workerStart();
}

void DapTunAndroid::workerStop()
{
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
    onWorkerStopped();
}

void DapTunAndroid::workerPause()
{
    if ( ::write( breaker1, "\0", 1) <= 0){
        qCritical() <<"Can't write to the breaker's pipe!";
        return;
    }
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
