#ifndef DAPTUNWINDOWS_H
#define DAPTUNWINDOWS_H

#include "DapTunAbstract.h"

class DapTunWindows : public DapTunAbstract
{
    
public:
    DapTunWindows();
    
protected:
    bool dhcpEnabled;
    ulong metric_eth, metric_tun;
    QString upstreamResolved;
    QStringList dwDests;
    void tunDeviceCreate()      override;
    void tunDeviceDestroy()     override;
    void workerPrepare()        override;
    void onWorkerStarted()      override;
    void workerStop()           override;
    void signalWriteQueueProc() override;
};

#endif // DAPTUNWINDOWS_H
