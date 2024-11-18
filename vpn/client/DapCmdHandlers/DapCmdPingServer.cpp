#include "DapCmdPingServer.h"
#include <QDebug>
#include "DapHttpPing.h"

DapCmdPingServer::DapCmdPingServer(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::PING_SERVER, parent)
{

}

void DapCmdPingServer::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    QString host = params->value(DapJsonParams::toString(DapJsonParams::HOST)).toString();
    quint16 port = params->value(DapJsonParams::toString(DapJsonParams::PORT)).toInt();
    qDebug() << "[DapCmdPingServer] handle = " << params;

    if( !host.isEmpty() && port != QJsonValue::Undefined ) {

        DapHttpPing *pingChecker = new DapHttpPing(host, port);
        pingChecker->sendRequest(host, port);

        connect(pingChecker, &DapHttpPing::sigResponse, this, [pingChecker, this](qint64 time) {
            qDebug() << "[DapCmdPingServer] sigResponse = \t" << pingChecker->getHost() << " = " << pingChecker->getPort() << " = " << time;
            QJsonObject response;
            response["host"] = pingChecker->getHost();
            response["port"] = pingChecker->getPort();
            response["responseTime"] = time;
            sendCmd(&response);
            delete pingChecker;
        });

        connect(pingChecker, &DapHttpPing::sigNetworkError, this, [pingChecker, this](QString err) {
            QJsonObject response;
            QJsonObject errorObj;
            qDebug() << "[DapCmdPingServer] sigNetworkError = \t" << pingChecker->getHost() << " = " << pingChecker->getPort();
            errorObj["code"] = -32000;
            errorObj["message"] = "Server not response";
            response["port"] = pingChecker->getPort();
            response["host"] = pingChecker->getHost();
            response["error"] = errorObj;

            sendCmd(&response);

            qWarning() << "Ping error: " << "Server not response: -32000";
            delete pingChecker;
        });
    } else {
        qWarning() << "Empty value";
    }
}
