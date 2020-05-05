#include <QtDebug>
#include "DapCmdServersList.h"
#include "DapJsonParams.h"
#include "DapServersListRequester.h"

DapCmdServersList::DapCmdServersList(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_SERVERS_LIST, parent)
{
}

void DapCmdServersList::handle(const QJsonObject* params)
{
    Q_UNUSED(params)
    auto reply = DapServersListRequester::sendRequest(serversList().front());
    if (!reply) {
        sendSimpleError(-32002, "Network unavailable");
        return;
    }
    connect(reply, &DapServersListNetworkReply::sigResponse, [=](const QJsonDocument& servers) {
        auto arr = servers.array();
        if (arr.isEmpty()) {
            rotateList();
            sendSimpleError(-666, "Empty nodelist, try another CDB...");
        } else {
            QJsonObject obj;
            obj["servers"] = arr;
            sendCmd(&obj);
        }
    });

    /*auto reply = DapServersListRequester::sendRequestsAll(serversList());
    connect(reply, &DapServersListNetworkReply::sigResponse, [=](const QJsonDocument& servers) {
        auto arr = servers.array();
        if (arr.isEmpty()) {
            sendSimpleError(-666, "Empty nodelist, try another CDB...");
        } else {
            QJsonObject obj;
            obj["servers"] = arr;
            sendCmd(&obj);
        }
    });*/

    connect(reply, &DapServersListNetworkReply::sigParseResponseError, [=]{
        qWarning()<< "Bad response from server. Parse error";
        rotateList();
        sendSimpleError(-32001, "Bad response from server. Parse error");
    });

    connect(reply, &DapServersListNetworkReply::sigNetworkError, [=]{
        qWarning()<< "Network error: " << reply->errorString();
        rotateList();
        sendSimpleError(-32002, reply->errorString());
    });
}
