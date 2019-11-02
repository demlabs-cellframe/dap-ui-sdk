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
    Q_UNUSED(params);
    auto reply = DapServersListRequester::sendRequest(serversList() );
    connect(reply, &DapServersListNetworkReply::sigResponse, [=](const QJsonDocument& servers) {
        auto arr = servers.array();
        QJsonObject obj;
        obj["servers"] = arr;
        sendCmd(&obj);
    });

    connect(reply, &DapServersListNetworkReply::sigParseResponseError, [=]{
        qWarning()<< "Bad response from server. Parse error";
        sendSimpleError(-32001, "Bad response from server. Parse error");
    });

    connect(reply, &DapServersListNetworkReply::sigNetworkError, [=]{
        qWarning()<< "Network error: " << reply->errorString();
        sendSimpleError(-32002, reply->errorString());
    });
}
