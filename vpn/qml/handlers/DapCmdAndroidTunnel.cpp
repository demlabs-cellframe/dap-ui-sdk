#include "DapCmdAndroidTunnel.h"

DapCmdAndroidTunnel::DapCmdAndroidTunnel(QObject *parent)
    : DapCmdClientAbstract (DapJsonCmdType::CREATE_ANDROID_TUNNEL, parent)
{

}

void DapCmdAndroidTunnel::handleResult(const QJsonObject &result)
{
    auto addr = result["address"].toString();
    auto gw = result["gateway"].toString();
    auto socket = result["socket"].toInt();
    qDebug() << "Command to connect";
    emit asked(addr, gw, socket);
}

void DapCmdAndroidTunnel::handleError(int code, const QString &message)
{
    Q_UNUSED(code);
    Q_UNUSED(message);
    qInfo() << message;
}
