#include "DapCmdAndroidTunnel.h"


DapCmdAndroidTunnel::DapCmdAndroidTunnel(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::CREATE_ANDROID_TUNNEL, parent)
{
    
}

DapCmdAndroidTunnel::~DapCmdAndroidTunnel()
{
    
}

void DapCmdAndroidTunnel::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
}

void DapCmdAndroidTunnel::openTunnel(const QString &address, const QString &gateway, int socket)
{
    qDebug() << QString("openTunnel(%1,%2,%3)").arg(address).arg(gateway).arg(socket);
    QJsonObject tunnelData;
    tunnelData["address"] = address;
    tunnelData["gateway"] = gateway;
    tunnelData["socket"] = socket;
    sendCmd(&tunnelData);
}
