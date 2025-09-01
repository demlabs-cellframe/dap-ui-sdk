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
    void addNewUpstreamRoute(const QString &a_addr);

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
    ulong metric_eth;
    ulong metric_tun;
    
    // TAP adapter troubleshooting helper methods
    bool isRunAsAdmin();
    bool preTunCreationCheck();
    bool attemptTapRemediation();
    bool attemptAlternativeTunCreation();
};

#endif // DAPTUNWINDOWS_H
