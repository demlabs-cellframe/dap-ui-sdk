#ifndef DAPCMDANDROIDTUNNEL_H
#define DAPCMDANDROIDTUNNEL_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdAndroidTunnel : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdAndroidTunnel(QObject *parent = nullptr);
    ~DapCmdAndroidTunnel() override;

    void handle(const QJsonObject *params) override;
    void openTunnel(const QString &address, const QString &gateway, int socket);
};

#endif // DAPCMDANDROIDTUNNEL_H
