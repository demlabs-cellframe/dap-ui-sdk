#ifndef DAPTUNANDROID_H
#define DAPTUNANDROID_H

#include "DapTunAbstract.h"
#include "DapTunWorkerUnix.h"
#include <QTcpServer>

class DapTunAndroid : public DapTunAbstract
{
public:
    DapTunAndroid();
    void workerStart()      override;
private:
    QTcpServer *receiver;
    void tunDeviceCreate()      override;
    void tunDeviceDestroy()     override;
    void workerPrepare()        override;
    void onWorkerStarted()      override;
    void workerStop()           override;
    void signalWriteQueueProc() override;
    void onVpnEstablished();
    int breaker0,breaker1;

    DapTunWorkerUnix * tunWorkerAndroid;
};

#endif // DAPTUNANDROID_H
