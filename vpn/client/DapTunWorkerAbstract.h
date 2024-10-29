#pragma once

#include <QObject>

#include <QThread>
#include <QQueue>
#include <QReadWriteLock>
#include <QWaitCondition>
//#include "DapSockForwPacket.h"
//#include "DapStreamChChainNetSrvVpn.h"
#include "DapStreamChChainVpnPacket.h"

#define DAP_IP_MTU 0xFFFF

namespace Dap {
    namespace Stream {
        struct Packet;
    }
}
class DapTunAbstract;

class DapTunWorkerAbstract : public QObject
{
    Q_OBJECT
public:
    DapTunWorkerAbstract( DapTunAbstract * a_tun );
    uint8_t m_tunBuf[DAP_IP_MTU];
signals:
    void packetOut( Dap::Stream::Packet* );

    void bytesWrite(quint64);
    void bytesRead(quint64);

    void loopStarted();
    void loopStopped();
    void loopError(const QString&);
public slots:
    virtual void loop()=0;

    void setTunSocket(int a_tunSocket){ m_tunSocket = a_tunSocket; }
protected:
    virtual void procDataFromTun(void * a_buf, size_t a_bufSize);
    int tunSocket(){ return m_tunSocket;}
    DapTunAbstract * tun(){ return m_tun; }
    Dap::Stream::Packet* writeDequeue() {
        Dap::Stream::Packet* ret;
        m_writeQueueLock->lockForRead();
        if (m_writeQueue->length() > 0) {
            ret = m_writeQueue->dequeue();
        } else {
            ret = nullptr;
        }
        m_writeQueueLock->unlock();
        return ret;
    }
private:
    volatile int m_tunSocket;

    DapTunAbstract * m_tun;
    QQueue<Dap::Stream::Packet*>* m_writeQueue;
    QWaitCondition * m_writeQueueCond;
    QReadWriteLock* m_writeQueueLock;
};

