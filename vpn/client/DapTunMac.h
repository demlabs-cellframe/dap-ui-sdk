#ifndef DAPTUNMAC_H
#define DAPTUNMAC_H

#include "DapTunAbstract.h"
#include "DapTunWorkerMac.h"

class DapDNSController;

class DapTunMac : public DapTunAbstract
{
    Q_OBJECT
public:
    explicit DapTunMac();
    ~DapTunMac();

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
    DapTunWorkerMac *tunWorker;
    QThread *tunThread;
    int breaker0, breaker1;
    DapDNSController *m_dnsController;
};

#endif // DAPTUNMAC_H
