#ifndef DAPTUNWINDOWS_H
#define DAPTUNWINDOWS_H

#include "DapTunAbstract.h"

class DapTunWindows : public DapTunAbstract
{
    
public:
    DapTunWindows();
    void addNewUpstreamRoute(const QString&) override;
    void workerStart()          override;
protected:
    QThread *tunThread;
    bool dhcpEnabled;
    ulong metric_eth, metric_tun;
    QString upstreamResolved;
    void tunDeviceCreate()      override;
    void tunDeviceDestroy()     override;
    void workerPrepare()        override;
    void onWorkerStarted()      override;
    void workerStop()           override;
    void workerPause()          override;
    void signalWriteQueueProc() override;

    void saveRoutingTable();
    void restoreRoutingTable();
    void logRoutingTable();
    QStringList m_savedRoutingTable;
};

#endif // DAPTUNWINDOWS_H
