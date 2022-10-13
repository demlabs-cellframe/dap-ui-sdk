#include "DapCmdNode.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdNode::actionParam = "node_info";

DapCmdNode::DapCmdNode(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::NODE_INFO, parent)
{

}

DapCmdNode::~DapCmdNode()
{

}

void DapCmdNode::sendError(int code, QString messageError)
{
    QJsonObject response;
    QJsonObject nodeInfo;
    nodeInfo["status"] = "error";
    nodeInfo["error_message"] = messageError;
    nodeInfo["error_code"] = code;
    response[DapCmdNode::actionParam] = nodeInfo;
    sendCmd(&response);
}

void DapCmdNode::sendNodeInfo(QStringList walletsList)
{
    QJsonObject response;
    QJsonObject nodeInfo;
    QJsonArray jsonWalletsList;
    jsonWalletsList.fromStringList(walletsList);
    nodeInfo["status"] = "ok";
    nodeInfo["wallets"] = jsonWalletsList;
    response[DapCmdNode::actionParam] = nodeInfo;
    sendCmd(&response);
}

void DapCmdNode::handle(const QJsonObject* params)
{
    if(params->value(actionParam) != QJsonValue::Undefined)
    {
        if (params->value(actionParam).toString() == "request")
            emit nodeInfoRequest();
        if (params->value(actionParam).toString() == "stop")
            emit nodeInfoRequestStop();
        return;
    }
}
