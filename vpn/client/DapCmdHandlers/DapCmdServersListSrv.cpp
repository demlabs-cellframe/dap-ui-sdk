#include <QtDebug>
#include "DapCmdServersListSrv.h"
#include "DapJsonParams.h"
#include "DapNetworkReply.h"
#include "DapConnectClient.h"
#include "DapDataLocal.h"

DapCmdServersListSrv::DapCmdServersListSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_SERVERS_LIST, parent)
{
}

void DapCmdServersListSrv::handle(const QJsonObject* params)
{
    Q_UNUSED(params)
    auto reply = new DapNetworkReply();
    connect(reply, &DapNetworkReply::finished, this, [=] {
        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->getReplyData(), &jsonErr);
        if (jsonDoc.isNull() || !jsonDoc.isArray()) {
            qCritical() << "Can't parse server response to JSON: "<< jsonErr.errorString() << " on position "<< jsonErr.offset;
            //emit nextCdb();
            if (++DapDataLocal::instance()->m_cdbIter == DapDataLocal::instance()->cdbServersList().end()) {
                DapDataLocal::instance()->m_cdbIter = DapDataLocal::instance()->cdbServersList().begin();
            }
            sendSimpleError(-32001, "Bad response from server. Parse error");
        } else {
            auto arr = jsonDoc.array();
            if (arr.isEmpty()) {
                //emit nextCdb();
                if (++DapDataLocal::instance()->m_cdbIter == DapDataLocal::instance()->cdbServersList().end()) {
                    DapDataLocal::instance()->m_cdbIter = DapDataLocal::instance()->cdbServersList().begin();
                }
                sendSimpleError(-32003, "Empty nodelist, try another CDB...");
            } else {
                QJsonObject obj;
                obj["servers"] = arr;
                sendCmd(&obj);
                DapServerInfoList l;
                if (DapServerInfo::fromJSON(obj.value("servers").toArray(), l))
                    if (!l.isEmpty()){
                        emit updateNodesList(l);
                        emit sendCurrentServer();
                    }
            }
        }
    });

    connect(reply, &DapNetworkReply::sigError, this, [=] {
        emit nextCdb();
        sendSimpleError(reply->error(), reply->errorString());
    });
    auto it = DapDataLocal::instance()->m_cdbIter;
    DapConnectClient::instance()->request_GET (it->address, it->port, "nodelist", *reply);
}
