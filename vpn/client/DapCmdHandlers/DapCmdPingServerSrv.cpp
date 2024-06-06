#include "DapCmdPingServerSrv.h"
#include <QDebug>
#include "DapHttpPing.h"

DapCmdPingServerSrv::DapCmdPingServerSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::PING_SERVER, parent)
{

}

void DapCmdPingServerSrv::handle(const QJsonObject* params)
{

    QString host = params->value(DapJsonParams::toString(DapJsonParams::HOST)).toString();
    quint16 port = params->value(DapJsonParams::toString(DapJsonParams::PORT)).toInt();

    if( !host.isEmpty() && port != QJsonValue::Undefined ) {

        DapHttpPing *pingChecker = new DapHttpPing(host, port);
        pingChecker->sendRequest(host, port);

        connect(pingChecker, &DapHttpPing::sigResponse, [=](qint64 time) {
            QJsonObject response;
            response["host"] = pingChecker->getHost();
            response["port"] = pingChecker->getPort();
            response["responseTime"] = time;
            sendCmd(&response);
            delete pingChecker;
        });

        connect(pingChecker, &DapHttpPing::sigNetworkError, [=](QString err) {
            QJsonObject response;
            QJsonObject errorObj;

            errorObj["code"] = -32000;
            errorObj["message"] = "Server not response";
            response["port"] = pingChecker->getPort();
            response["host"] = pingChecker->getHost();
            response["error"] = errorObj;

            sendCmd(&response);

            qWarning() << "Ping error: " << err;
            delete pingChecker;
        });
    } else {
        qWarning() << "Empty value";
    }
}
