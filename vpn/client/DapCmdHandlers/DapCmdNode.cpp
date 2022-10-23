#include "DapCmdNode.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdNode::actionParam = "node_data";

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
    if(params->value(actionParam) != QJsonValue::Undefined && params->value(actionParam).isObject())
    {
        QJsonObject nodeCmd = params->value(actionParam).toObject();
        // start checking the operation of the node
        if (nodeCmd["check_node_request"].isBool() && nodeCmd["check_node_request"].toBool())
            emit startCheckingNode();
        // data wallet request
        if (nodeCmd["data_wallet_request"].isBool() && nodeCmd["data_wallet_request"].toBool())
            emit dataWalletRequest();
        // stop checking the operation of the node
        if (nodeCmd["check_node_request"].isBool() && !nodeCmd["check_node_request"].toBool())
            emit stopCheckingNode();
        // creating a conditional transaction cmd
        if (nodeCmd["cond_tx_create"].isObject())
        {
            QJsonObject tx = nodeCmd["cond_tx_create"].toObject();
            QString tokenName   = tx["token_name"].toString();
            QString walletName  = tx["wallet_name"].toString();
            QString certName    = tx["cert_name"].toString();
            qreal value         = tx["value"].toDouble();
            emit condTxCreateRequest(tokenName, walletName, certName, value);
        }
    }
}
