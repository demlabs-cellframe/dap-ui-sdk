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
        reply->deleteLater();
        sendSimpleError(-32000, "Network unavailable");
        qWarning()<< "Network unavailable, do nothing";
        return;
    }
    connect(reply, &DapServersListNetworkReply::sigResponse, [=](const QJsonDocument& servers) {
        reply->deleteLater();
        auto arr = servers.array();
        if (arr.isEmpty()) {
            rotateList();
            sendSimpleError(-666, "Empty nodelist, try another CDB...");
        } else {
            QJsonObject obj;
            obj["servers"] = arr;
            sendCmd(&obj);
            DapServerInfoList l;
            if (DapServerInfo::fromJSON(obj.value("servers").toArray(), l))
                if (!l.isEmpty())
                    emit updateNodesList(l);
        }
    });

    connect(reply, &DapServersListNetworkReply::sigParseResponseError, [=]{
        reply->deleteLater();
        qWarning()<< "Bad response from server. Parse error";
        rotateList();
        sendSimpleError(-32001, "Bad response from server. Parse error");
    });

    connect(reply, &DapServersListNetworkReply::sigNetworkError, [=](DapNetworkReply::DapNetworkError err){
        reply->deleteLater();
        qWarning()<< "Network error: " << err;
        rotateList();
        if (err == DapNetworkReply::UnknownNetworkError)
            sendSimpleError(-32003, "Unknown error");
        else
            sendSimpleError(-32002, "Error fetching nodelist");
    });
}
