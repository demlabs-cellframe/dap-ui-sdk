#ifndef DAPTUNMAC_H
#define DAPTUNMAC_H

#include "DapTunAbstract.h"

class DapTunWorkerMac;

class DapTunMac : public DapTunAbstract
{
public:
    DapTunMac();
    void addNewUpstreamRoute(const QString&);
    ~DapTunMac();
protected:
    void tunDeviceCreate();
    void tunDeviceDestroy();
    void onWorkerStarted();

    void workerPrepare();
    void workerStop();
    void workerPause();
    void signalWriteQueueProc();

    void backupAndApplyDNS();
    void getBackDNS();

    int breaker0,breaker1;
    DapTunWorkerMac * tunWorkerMac;

};

#endif // DAPTUNMAC_H
