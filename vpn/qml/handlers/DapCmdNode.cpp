
#include "DapCmdNode.h"
#include <QDebug>

#define DEBUGINFO qDebug()<<"-----<<<-----"

// ui side

const QString DapCmdNode::actionParam = "node_data";
void DapCmdNode::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains(DapCmdNode::actionParam))
    {
        DEBUGINFO << "Node info" << *params << params->value(DapCmdNode::actionParam) << params->value(DapCmdNode::actionParam).isObject();
        if (params->value(DapCmdNode::actionParam).isObject())
        {
            QJsonObject nodeData = params->value(DapCmdNode::actionParam).toObject();
            if (nodeData["status"].toString() == QString("ok"))
            {
                DEBUGINFO << "status ok";
                m_hasError = false;
                if (nodeData["wallets"].isArray())
                {
                    QJsonArray array = nodeData["wallets"].toArray();
                    QStringList stringList;
                    foreach( QVariant vItem, array )
                        stringList << vItem.toString();
                    DEBUGINFO << "wallets" << stringList;
                    emit walletsList(stringList);
                    return;
                }
                if (nodeData["networks"].isArray())
                {
                    QJsonArray array = nodeData["networks"].toArray();
                    QStringList stringList;
                    foreach( QVariant vItem, array )
                        stringList << vItem.toString();
                    DEBUGINFO << "networks" << stringList;
                    emit networksList(stringList);
                    return;
                }
                if (nodeData["node_detected"].isBool() && nodeData["node_detected"].toBool())
                {
                    DEBUGINFO << "node detected";
                    emit nodeDetected();
                    return;
                }
            }
            else
            {
                m_hasError = true;
                QString errorMessage = nodeData["error_message"].toString();
                int errorCode = nodeData["error_code"].toInt();
                emit nodeError(errorCode, errorMessage);
                return;
            }
        }
    }
}

void DapCmdNode::startNodeDetection()
{
    QJsonObject checkNode;
    checkNode["start_node_detection"] = true;
    sendRequest(checkNode);
}

void DapCmdNode::stopCheckNode()
{
    QJsonObject checkNode;
    checkNode["start_node_detection"] = false;
    sendRequest(checkNode);
}

void DapCmdNode::condTxCreate(QString tokenName, QString walletName, QString certName, qreal value)
{
    QJsonObject condTx;
    condTx["token_name"] = tokenName;
    condTx["wallet_name"] = walletName;
    condTx["cert_name"] = certName;
    condTx["value"] = value;
    sendRequest(condTx);
}


void DapCmdNode::sendRequest(const QJsonObject &data)
{
    QJsonObject request;
    request[DapCmdNode::actionParam] = data;
    sendCmd(&request);
}
