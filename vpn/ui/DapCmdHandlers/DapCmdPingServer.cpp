#include "DapCmdPingServer.h"

#include "DapJsonParams.h"
#include <QDebug>

DapCmdPingServer::DapCmdPingServer(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::PING_SERVER, parent)
{

}

void DapCmdPingServer::sendPingCmd(const QString& host, quint16 port)
{
    qDebug() << "Cmd ping: " << host << " " << port;
    QJsonObject obj;
    obj["host"] = host;
    obj["port"] = port;
    sendCmd(&obj);
}

void DapCmdPingServer::handleResult(const QJsonObject& result)
{
    QString host = result.value(DapJsonParams::toString(DapJsonParams::HOST)).toString();
    quint16 port = result.value(DapJsonParams::toString(DapJsonParams::PORT)).toInt();
    quint16 ping = result.value("responseTime").toInt()/2;
    qDebug() << host << ":" << port << "Ping result:" << ping << "ms" ;
    emit sigPingReceived(host, port, ping);
}

void DapCmdPingServer::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
