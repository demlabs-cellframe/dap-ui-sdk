#pragma once

#include <QObject>
#include "DapTunAbstract.h"

class DapTunWorkerUnix;

class DapTunUnixAbstract : public DapTunAbstract
{
    Q_OBJECT
public:
    DapTunUnixAbstract();
    void addNewUpstreamRoute(const QString&) override;
    void workerStart() override;
protected:
    QThread *tunThread;
    QString m_ethDevice; // eth adapter device name
    QString m_TunDeviceH;// tunDevice name human freendly name

    void tunDeviceCreate() override{}
    void workerStop() override;
    void workerPause() override;
    void workerPrepare() override;
    void tunDeviceDestroy() override;
    void signalWriteQueueProc() override;

    int breaker0,breaker1;
    DapTunWorkerUnix * tunWorkerUnix;

};

