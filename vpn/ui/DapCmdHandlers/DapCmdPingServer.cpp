#include "DapCmdPingServer.h"

#include "DapJsonParams.h"
#include <QDebug>

DapCmdPingServer::DapCmdPingServer(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::PING_SERVER, parent)
{

}

void DapCmdPingServer::sendPingCmd(const QString& host, quint16 port)
{
    QJsonObject obj;
    obj["host"] = host;
    obj["port"] = port;
    sendCmd(&obj);
}

void DapCmdPingServer::handleResult(const QJsonObject& result)
{
    QJsonValue ping = result.value("responseTime");
    QString host = result.value(DapJsonParams::toString(DapJsonParams::HOST)).toString();
    quint16 port = result.value(DapJsonParams::toString(DapJsonParams::PORT)).toInt();
    qDebug() << "Ping result:" << ping.toInt() << "ms" << host << " " << port;
}

void DapCmdPingServer::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
