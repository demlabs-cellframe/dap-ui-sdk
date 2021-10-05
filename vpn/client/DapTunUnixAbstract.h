#pragma once

#include <QObject>
#include "DapTunAbstract.h"

class DapTunWorkerUnix;

class DapTunUnixAbstract : public DapTunAbstract
{
    Q_OBJECT
public:
    DapTunUnixAbstract();
    void addNewUpstreamRoute(const QString&);
protected:
    QThread *tunThread;
    QString m_ethDevice; // eth adapter device name
    QString m_TunDeviceH;// tunDevice name human freendly name
    void tunDeviceCreate(){;}
    void workerStart();
    void workerStop();
    void workerPause();
    void workerPrepare();
    void tunDeviceDestroy();
    void signalWriteQueueProc();

    int breaker0,breaker1;
    DapTunWorkerUnix * tunWorkerUnix;

};

