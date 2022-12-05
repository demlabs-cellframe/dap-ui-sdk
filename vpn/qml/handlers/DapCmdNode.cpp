
#include "DapCmdNode.h"
#include <QDebug>

#define DEBUGINFO qDebug()<<"--->UiCMD<---"

// ui side


const QString DapCmdNode::actionParam = "node_data";
void DapCmdNode::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
    if(params->contains(DapCmdNode::actionParam))
    {
        // node data parsing
        DEBUGINFO << "Node info" << *params << params->value(DapCmdNode::actionParam) << params->value(DapCmdNode::actionParam).isObject();
        if (params->value(DapCmdNode::actionParam).isObject())
        {
            QJsonObject nodeData = params->value(DapCmdNode::actionParam).toObject();
            // data status check
            if (nodeData["status"].toString() == QString("ok"))
            {
                DEBUGINFO << "status ok";
                m_hasError = false;
                // wallets list
                if (nodeData["wallets"].isArray())
                {
                    QJsonArray array = nodeData["wallets"].toArray();
                    QMap<QString, QVariant> data;
                    foreach(const QVariant& vItem, array)
                        data[vItem.toString()] = "";
                    DEBUGINFO << "wallets" << data;
//                    emit walletsList(data);
                    return;
                }
                // networks list
                if (nodeData["networks"].isArray())
                {
                    QJsonArray array = nodeData["networks"].toArray();
                    QMap<QString, QVariant> data;
                    foreach(const QVariant& vItem, array)
                        data[vItem.toString()] = "";
                    DEBUGINFO << "networks" << data;
//                    emit networksList(data);
                    return;
                }
                // wallets data
                if (nodeData["wallets_data"].isObject())
                {
                    DEBUGINFO << "wallets_data" << nodeData["wallets_data"];
                    m_dataWallet.setData(nodeData["wallets_data"].toObject());
                    emit walletsList(m_dataWallet.walletsWithTokens());
                    return;
                }
                // node detected status
                if (nodeData["node_detected"].isBool() && nodeData["node_detected"].toBool())
                {
                    DEBUGINFO << "node detected";
                    emit nodeDetected();
                    return;
                }
                // transaction hash in mempool
                if (nodeData["transaction_hash_in_mempool"].isBool() && nodeData["transaction_hash_in_mempool"].toBool())
                {
                    DEBUGINFO << "transaction hash in mempool";
                    emit transactionHashInMempool();
                    return;
                }
                // transaction hash in ledger
                if (nodeData["transaction_hash_in_ledger"].isBool() && nodeData["transaction_hash_in_ledger"].toBool())
                {
                    DEBUGINFO << "transaction hash in ledger";
                    emit transactionHashInledger();
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

void DapCmdNode::condTxCreate()
{
    QJsonObject condTx;
    condTx["wallet_name"] = m_selectedWalletName;
    condTx["network_name"] = m_selectedNetworkName;
    condTx["token_name"] = m_selectedTokenName;
//    condTx["cert_name"] = certName;
    condTx["value"] = m_value;
    condTx["unit"] = m_unit;
    QJsonObject jObject;
    jObject["cond_tx_create"] = condTx;
    sendRequest(jObject);
}


void DapCmdNode::sendRequest(const QJsonObject &data)
{
    QJsonObject request;
    request[DapCmdNode::actionParam] = data;
    sendCmd(&request);
}

void DapCmdNode::chooseWallet(QString wallet)
{
    m_selectedWalletName = wallet;
    emit networksList(nodeInfo()->networkWithTokens(wallet));
}

void DapCmdNode::chooseNetwork(QString network)
{
    m_selectedNetworkName = network;
    emit tokensInfo(nodeInfo()->tokensAmount(m_selectedWalletName, m_selectedNetworkName));
}

void DapCmdNode::chooseToken(QString token)
{
    m_selectedTokenName = token;
    auto tokens = nodeInfo()->tokensAmount(m_selectedWalletName, m_selectedNetworkName);
    emit tokenAmount(token, tokens[token].toString());
}


void DapCmdNode::setValue(QString value)
{
    m_value = value;
}
