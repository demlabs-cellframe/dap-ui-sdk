#include "DapCmdPingServer.h"
#include <QDebug>
#include "DapHttpPing.h"

DapCmdPingServer::DapCmdPingServer(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::PING_SERVER, parent)
{

}

void DapCmdPingServer::handle(const QJsonObject* params)
{
    QString host = params->value(DapJsonParams::toString(DapJsonParams::HOST)).toString();

    QJsonValue port = params->value(DapJsonParams::toString(DapJsonParams::PORT));
    if(!host.isEmpty() && port != QJsonValue::Undefined) {
        auto c = DapHttpPing::sendRequest(host, port.toInt());
        connect(c, &DapHttpPingNetworkReply::sigResponse, [=](qint64 time) {
            QJsonObject response;
            response["host"] = c->getHost();
            response["port"] = c->getPort();
            response["responseTime"] = time;

            sendCmd(&response);
        });

        connect(c, &DapHttpPingNetworkReply::sigNetworkError, [=](QNetworkReply::NetworkError err) {
            QJsonObject response;
            QJsonObject errorObj;

            errorObj["code"] = -32000;
            errorObj["message"] = "Server not response";
            response["port"] = c->getPort();
            response["host"] = c->getHost();
            response["error"] = errorObj;

            sendCmd(&response);

            qWarning() << "Ping error: " << err;
        });
    } else {
        qWarning() << "Empty value";
    }
}
