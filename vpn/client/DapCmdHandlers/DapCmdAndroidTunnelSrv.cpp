#include "DapCmdAndroidTunnelSrv.h"


DapCmdAndroidTunnelSrv::DapCmdAndroidTunnelSrv(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::CREATE_ANDROID_TUNNEL, parent)
{
    
}

DapCmdAndroidTunnelSrv::~DapCmdAndroidTunnelSrv()
{
    
}

void DapCmdAndroidTunnelSrv::handle(const QJsonObject *params)
{
    Q_UNUSED(params)
}

void DapCmdAndroidTunnelSrv::openTunnel(const QString &address, const QString &gateway, int socket)
{
    qDebug() << QString("openTunnel(%1,%2,%3)").arg(address).arg(gateway).arg(socket);
    QJsonObject tunnelData;
    tunnelData["address"] = address;
    tunnelData["gateway"] = gateway;
    tunnelData["socket"] = socket;
    sendCmd(&tunnelData);
}
