#include "DapCmdNode.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

// client, service side
#define DEBUGINFO qDebug()<<"-----<<<<<---"

const QString DapCmdNode::actionParam = "node_data";

DapCmdNode::DapCmdNode(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::NODE_INFO, parent),
      m_nodeDetected(false)
{

}

DapCmdNode::~DapCmdNode()
{

}

void DapCmdNode::setNodeDetected()
{
    DEBUGINFO << "slot setNodeDetected";
    m_nodeDetected = true;
    sendNodeDetected();
}

void DapCmdNode::sendNodeDetected()
{
    QJsonObject response;
    QJsonObject nodeInfo;
    nodeInfo["status"] = "ok";
    nodeInfo["node_detected"] = m_nodeDetected;
    response[DapCmdNode::actionParam] = nodeInfo;
    sendCmd(&response);
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

void DapCmdNode::sendWalletsList(QStringList walletsList)
{
    QJsonObject response;
    QJsonObject nodeInfo;
    QJsonArray jsonWalletsList;
    jsonWalletsList.fromStringList(walletsList);
    nodeInfo["status"] = "ok";
    foreach (QString data, walletsList)
        jsonWalletsList.append(data);
    nodeInfo["wallets"] = jsonWalletsList;
    response[DapCmdNode::actionParam] = nodeInfo;
    sendCmd(&response);
    DEBUGINFO << "sendWalletsList" << walletsList << response;
}

void DapCmdNode::sendNetworksList(QStringList walletsList)
{
    QJsonObject response;
    QJsonObject nodeInfo;
    QJsonArray jsonWalletsList;
    jsonWalletsList.fromStringList(walletsList);
    nodeInfo["status"] = "ok";
    foreach (QString data, walletsList)
        jsonWalletsList.append(data);
    nodeInfo["networks"] = jsonWalletsList;
    response[DapCmdNode::actionParam] = nodeInfo;
    sendCmd(&response);
    DEBUGINFO << "sendNetworksList" << walletsList << response;
}

void DapCmdNode::handle(const QJsonObject* params)
{
    if(params->value(actionParam) != QJsonValue::Undefined && params->value(actionParam).isObject())
    {
        QJsonObject nodeCmd = params->value(actionParam).toObject();
        // start checking the operation of the node
        if (nodeCmd["start_node_detection"].isBool() && nodeCmd["start_node_detection"].toBool())
            emit startNodeDetection();
        // data wallet request
        if (nodeCmd["data_wallet_request"].isBool() && nodeCmd["data_wallet_request"].toBool())
            emit dataWalletRequest();
        // stop checking the operation of the node
        if (nodeCmd["start_node_detection"].isBool() && !nodeCmd["start_node_detection"].toBool())
            emit stopNodeDetection();
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
        if (nodeCmd["node_detected_check"].isBool() && nodeCmd["node_detected_check"].toBool())
            sendNodeDetected();
    }
}
