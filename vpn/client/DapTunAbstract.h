#pragma once

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QReadWriteLock>
#include <pthread.h>

#include "DapSession.h"
#include "DapTunWorkerAbstract.h"
#include "DapStreamChChainVpnPacket.h"
#ifdef ANDROID
#include <QFuture>
#endif

class DapTunAbstract : public QObject
{
    Q_OBJECT
public:
    DapTunAbstract();

    void create(const QString &a_addr, const QString &a_gw,
                const QString & a_upstreamAddress = QString(), qint16 a_upstreamPort = 0,
                int a_upstreamSocket = -1, QStringList a_routing_exceptions = QStringList());
    void destroy();
    void standby();
    void setTunSocket(int a_tunSocket){ m_tunSocket = a_tunSocket; }
    void setTunDeviceName(const QString& a_tunDeviceName){ m_tunDeviceName = a_tunDeviceName; }
    bool isCreated();
    const QString& addr() { return m_addr; }
    const QString& gw() { return m_gw; }
    int upstreamSocket() { return m_upstreamSocket; }
    const QString& tunDeviceName(){ return m_tunDeviceName; }

    void tunWriteData(Dap::Stream::Packet *a_pkt) {
        addWriteData(a_pkt);
        signalWriteQueueProc();
    }
    virtual void workerStart() = 0;
    virtual void addNewUpstreamRoute(const QString&)=0;
    virtual void addCdbRoute(const QString &cdbAddress) = 0;

    QQueue<Dap::Stream::Packet*>* writeQueue(){ return &_m_writeQueue; }
    QReadWriteLock* writeQueueLock(){ return &m_writeQueueLock; }
    QWaitCondition * writeQueueCond() { return &m_writeQueueCond; }

    volatile int m_tunSocket;

    Dap::Stream::Packet* writeDequeuePacket() {
        Dap::Stream::Packet* ret;
        m_writeQueueLock.lockForRead();
        if (_m_writeQueue.length() > 0) {
            ret = _m_writeQueue.dequeue();
        } else {
            ret = nullptr;
        }
        m_writeQueueLock.unlock();
        return ret;
    }
    
signals:
    void nativeCreateRequest();
    void nativeDestroyRequest();
    
    void created();
    void destroyed();
    void error(const QString&);

    void sendCmd(QString);

    void packetOut(Dap::Stream::Packet *);

    void bytesRead(quint64);
    void bytesWrite(quint64);

    void sigStartWorkerLoop();
protected:
    virtual void tunDeviceCreate()=0;
    virtual void tunDeviceDestroy()=0;
    virtual void workerPrepare()=0;
    virtual void workerStop()=0;
    virtual void workerPause() = 0;
    //virtual void workerResume() = 0;
    virtual void signalWriteQueueProc()=0;

    void initWorker();
    void readNetrowkInformFromFile();
    bool isLocalAddress(const QString &address);

    QString m_gwOld;
    DapTunWorkerAbstract * tunWorker;
    bool m_isCreated;

    QString m_tunDeviceName; // tunDevice name
    QString m_tunDeviceReg;
    QString m_tunDeviceHuman;
    QString m_ethDeviceName;
    QString m_ethDevice;
    QString m_defaultGwOld;
    QList<QString> m_routingExceptionAddrs;

private:
    const QString tempNetFileName = "TempConfigurationNetwork.xml";

    int m_upstreamSocket;
    pthread_t pidTun;

    QQueue<Dap::Stream::Packet*> _m_writeQueue;
    //QQueue<DapSockForwPacket*> m_writeQueue;
    QReadWriteLock m_writeQueueLock;
    QWaitCondition m_writeQueueCond;


protected:
    QString m_addr /* m_tunAddr */, m_gw /*m_tunDest*/;
    /// Upstream address.
    QString m_sUpstreamAddress;
    /// Upstream port.
    qint16  m_iUpstreamPort {0};

protected slots:
    /*void addWriteData(DapSockForwPacket* a_pkt){
        m_writeQueueLock.lockForWrite();
        m_writeQueue.enqueue(a_pkt);
        m_writeQueueLock.unlock();
    }*/

    void addWriteData(Dap::Stream::Packet* a_pkt){
        m_writeQueueLock.lockForWrite();
        _m_writeQueue.enqueue(a_pkt);
        m_writeQueueLock.unlock();
    }

    virtual void onWorkerStarted();
    virtual void onWorkerStopped();

    virtual void afterTunDeviceDestroyed();
    
public slots:
    /// Get upstream address.
    /// @return Upstream address.
    QString upstreamAddress() const;
    /// Set upstream address.
    /// @param arsUpstreamAddress Upstream address.
    void setUpstreamAddress(const QString &arsUpstreamAddress);
    /// Get upstream port.
    /// @return Upstream port.
    qint16 upstreamPort() const;
    /// Set upstream port.
    /// @param aiUpstreamPort Upstream port.
    void setUpstreamPort(qint16 aiUpstreamPort);
};
