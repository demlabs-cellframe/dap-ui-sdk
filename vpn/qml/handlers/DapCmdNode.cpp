
#include "DapCmdNode.h"

const QString DapCmdNode::actionParam = "node_info";
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

void DapCmdNode::sendNodeInfoRequest()
{
    QJsonObject response;
    response[DapCmdNode::actionParam] = "request";
    sendCmd(&response);
}

void DapCmdNode::sendNodeInfoRequestStop()
{
    QJsonObject response;
    response[DapCmdNode::actionParam] = "stop";
    sendCmd(&response);
}
