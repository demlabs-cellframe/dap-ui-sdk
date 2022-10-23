
#include "DapCmdNode.h"

const QString DapCmdNode::actionParam = "node_data";
void DapCmdNode::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains(DapCmdNode::actionParam))
    {
        qDebug() << "Node info" << params->value(DapCmdNode::actionParam).toString();
        if (params->value(DapCmdNode::actionParam).isObject())
        {
            QJsonObject nodeData = params->value(DapCmdNode::actionParam).toObject();
            if (nodeData["satus"] == "ok")
            {
                m_hasError = false;
                if (nodeData["wallets"].isArray())
                {
                    QJsonArray array = nodeData["wallets"].toArray();
                    QStringList stringList;
                    foreach( QVariant vItem, array )
                        stringList << vItem.toString();
                    emit walletsList(stringList);
                }
            }
            else
            {
                m_hasError = true;
                QString errorMessage = nodeData["error_message"].toString();
                int errorCode = nodeData["error_code"].toInt();
                emit nodeError(errorCode, errorMessage);
            }
        }
    }
}

void DapCmdNode::startCheckNode()
{
    QJsonObject checkNode;
    checkNode["check_node_request"] = true;
    sendRequest(checkNode);
}

void DapCmdNode::stopCheckNode()
{
    QJsonObject checkNode;
    checkNode["check_node_request"] = false;
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
