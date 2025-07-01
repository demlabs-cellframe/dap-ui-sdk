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
    QString instanceId = params->value(DapJsonParams::toString(DapJsonParams::INSTANCE_ID)).toString();

    if( !host.isEmpty() && port != QJsonValue::Undefined ) {

        DapHttpPing *pingChecker = new DapHttpPing(host, port);
        pingChecker->sendRequest(host, port);

        connect(pingChecker, &DapHttpPing::sigResponse, this, [pingChecker, instanceId, this](qint64 time) {
            QJsonObject response;
            response["host"] = pingChecker->getHost();
            response["port"] = pingChecker->getPort();
            response["instance_id"] = instanceId;
            response["responseTime"] = time;
            sendCmd(&response);
            delete pingChecker;
        });

        connect(pingChecker, &DapHttpPing::sigNetworkError, this, [pingChecker, this](QString err) {
            QJsonObject response;
            QJsonObject errorObj;

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
