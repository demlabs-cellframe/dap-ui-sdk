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
        sendSimpleError(-32000, "Network unavailable");
        qWarning()<< "Network unavailable, do nothing";
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
            DapServerInfoList l;
            if (DapServerInfo::fromJSON(obj.value("servers").toArray(), l))
                if (!l.isEmpty())
                    emit updateNodesList(l);
        }
    });

    connect(reply, &DapServersListNetworkReply::sigParseResponseError, [=]{
        qWarning()<< "Bad response from server. Parse error";
        rotateList();
        sendSimpleError(-32001, "Bad response from server. Parse error");
    });

    connect(reply, &DapServersListNetworkReply::sigNetworkError, [=]{
        qWarning()<< "Network error: " << reply->errorString();
        rotateList();
        if (reply->errorString() == "Network unreachable" || reply->errorString() == "Network access is disabled.")
            sendSimpleError(-32003, reply->errorString());
        else
            sendSimpleError(-32002, reply->errorString());
    });
}
