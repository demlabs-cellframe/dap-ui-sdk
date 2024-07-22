#ifndef DAPCMDANDROIDTUNNEL_H
#define DAPCMDANDROIDTUNNEL_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdAndroidTunnelSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdAndroidTunnelSrv(QObject *parent = nullptr);
    ~DapCmdAndroidTunnelSrv() override;

    void handle(const QJsonObject *params) override;
    void openTunnel(const QString &address, const QString &gateway, int socket);
};

#endif // DAPCMDANDROIDTUNNEL_H
