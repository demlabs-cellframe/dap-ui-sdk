#ifndef DAPTUNWINDOWS_H
#define DAPTUNWINDOWS_H

#include "DapTunAbstract.h"
#include "DapTunWorkerWindows.h"

class DapDNSController;

class DapTunWindows : public DapTunAbstract
{
    Q_OBJECT
public:
    explicit DapTunWindows();
    ~DapTunWindows();

    void tunDeviceCreate() override;
    void tunDeviceDestroy() override;
    void workerPrepare() override;
    void workerStart() override;
    void workerStop() override;
    void workerPause() override;
    void signalWriteQueueProc() override;

protected:
    void onWorkerStarted() override;
    void onWorkerStopped() override;

private:
    DapTunWorkerWindows *tunWorker;
    QThread *tunThread;
    QString m_defaultGwOld;
    QString m_ethDeviceName;
    QString m_tunDeviceReg;
    QString upstreamResolved;
    DapDNSController *m_dnsController;
};

#endif // DAPTUNWINDOWS_H
