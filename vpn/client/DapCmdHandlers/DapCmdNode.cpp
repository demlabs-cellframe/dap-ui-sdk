#include "DapCmdNode.h"
#include <QMap>
#include <QDebug>
#include "DapNode.h"

// client, service side
#define DEBUGINFO qDebug()<<"--->SrvCMD<---"

DapCmdNode::DapCmdNode(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::NODE_INFO, parent),
      m_nodeDetected(false),
      m_nocdbMode(false)
{

}

DapCmdNode::~DapCmdNode()
{

}

void DapCmdNode::setNodeDetected()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_nodeDetected = true;
    sendNodeDetected();
}

void DapCmdNode::sendNoCdbMode()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["nocdb_mode"] = m_nocdbMode;
    sendCmd(&response);
}

void DapCmdNode::sendNodeDetected()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["node_detected"] = m_nodeDetected;
    sendCmd(&response);
}

void DapCmdNode::sendError(int code, const QString& messageError)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    QJsonObject nodeInfo;
    nodeInfo["message"] = messageError;
    nodeInfo["code"] = code;
    response["error"] = nodeInfo;
    sendCmd(&response);
}

void DapCmdNode::sendWalletsList(const QStringList& walletsList)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    QJsonArray jsonWalletsList;
    jsonWalletsList.fromStringList(walletsList);
    for (const QString& data: walletsList)
        jsonWalletsList.append(data);
    response["wallets"] = jsonWalletsList;
    sendCmd(&response);
    DEBUGINFO << "sendWalletsList";
}

void DapCmdNode::sendNetworksList(const QStringList& walletsList)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    QJsonArray jsonWalletsList;
    jsonWalletsList.fromStringList(walletsList);
    for (const QString& data: walletsList)
        jsonWalletsList.append(data);
    response["networks"] = jsonWalletsList;
    sendCmd(&response);
    DEBUGINFO << "sendNetworksList";
}

void DapCmdNode::sendWalletsData(const QJsonObject& a_walletsData)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["wallets_data"] = a_walletsData;
    sendCmd(&response);
    DEBUGINFO << "sendWalletsData";
}

QJsonObject DapCmdNode::transformKeys(const QJsonObject& inputObj) {
    QJsonObject result;
    for (const QString& key : inputObj.keys()) {
        QString newKey = key;
        newKey.replace(' ', '_');

        if (inputObj[key].isObject()) {
            result[newKey] = transformKeys(inputObj[key].toObject());
        } else if (inputObj[key].isArray()) {
            QJsonArray newArray;
            for (const QJsonValue& value : inputObj[key].toArray()) {
                if (value.isObject()) {
                    newArray.append(transformKeys(value.toObject()));
                } else {
                    newArray.append(value);
                }
            }
            result[newKey] = newArray;
        } else {
            result[newKey] = inputObj[key];
        }
    }
    return result;
}

void DapCmdNode::sendOrderList(const QJsonArray& orderList) {
    DEBUGINFO << __PRETTY_FUNCTION__;
    qDebug() << "🔍 [FILTER DEBUG] DapCmdNode::sendOrderList: Input orders count =" << orderList.size();

    QJsonArray transformedOrderList;
    for (const auto& order : orderList) {
        QJsonObject transformedOrder = transformKeys(order.toObject());
        qDebug() << transformedOrder["hash"] << " " << transformedOrder["node_addr"] << " " << transformedOrder["node_location"];
        transformedOrderList.append(transformedOrder);
    }

    qDebug() << "🔍 [FILTER DEBUG] DapCmdNode::sendOrderList: Transformed orders count =" << transformedOrderList.size();
    
    QJsonObject response;
    response["order_list"] = transformedOrderList;
    sendCmd(&response);
    DEBUGINFO << "sendOrderList";
}

void DapCmdNode::sendNodeIp(const QJsonObject& nodeIpList)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["node_ip_list"] = nodeIpList;
    sendCmd(&response);
    DEBUGINFO << "sendNodeIpList";
}



void DapCmdNode::sendTransactionInMempool()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["transaction_hash_in_mempool"] = true;
    sendCmd(&response);
    DEBUGINFO << "sendTransactionInMempool";
}

void DapCmdNode::sendTransactionInLedger()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    response["transaction_hash_in_ledger"] = true;
    sendCmd(&response);
    DEBUGINFO << "sendTransactionInLedger";
}

void DapCmdNode::sendSigningInfo(qint32 utype, qint64 uid, qint64 units, QString price)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject response;
    QJsonObject info;
    info["utype"] = utype;
    info["uid"] = uid;
    info["units"] = QString::number(units);
    info["value"] = price;
    response["signing_info"] = info;
    sendCmd(&response);
    DEBUGINFO << "sendSigningInfo" << utype << QString::number(units) << price;
}

void DapCmdNode::sendFeeData (const QJsonObject &a_data)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  QJsonObject response;
  response["get_fee_data"] = a_data;
  sendCmd(&response);
  DEBUGINFO << "sendFeeData" << a_data;
}

void DapCmdNode::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    DEBUGINFO << __PRETTY_FUNCTION__ << *params;

    if (params->contains("start_node_detection") && params->value("start_node_detection").toBool()) {
        qDebug() << "Emitting sigStartNodeDetection";
        emit sigStartNodeDetection();
    }

    if (params->contains("nocdb_mode_request") && params->value("nocdb_mode_request").toBool()) {
        qDebug() << "Emitting sendNoCdbMode";
        sendNoCdbMode();
    }

    // Data wallet request
    if (params->contains("data_wallet_request") && params->value("data_wallet_request").toBool()) {
        qDebug() << "Emitting dataWalletRequest";
        emit dataWalletRequest();
    }

    // Stop checking the operation of the node
    if (params->contains("start_node_detection") && !params->value("start_node_detection").toBool()) {
        qDebug() << "Emitting stopNodeDetection";
        emit stopNodeDetection();
    }

    // Creating a conditional transaction
    if (params->contains("cond_tx_create"))
    {
        QJsonObject tx = params->value("cond_tx_create").toObject();
        QString walletName  = tx.value("wallet_name").toString();
        QString networkName = tx.value("network_name").toString();
        QString tokenName   = tx.value("token_name").toString();
        QString value       = tx.value("value").toString();
        QString unit        = tx.value("unit").toString();

        qDebug() << "cond_tx_create - " << "networkName: " << networkName << " walletName: " << walletName << " tokenName: " << tokenName << " unit: " << unit << " value: " << value;

        // Checking the integrity of transaction creation parameters
        if (networkName.isEmpty() || walletName.isEmpty() || tokenName.isEmpty() || value.isEmpty() || unit.isEmpty()){
            qDebug() << "Corrupted transaction creation parameters";
            sendError(732, "Corrupted transaction creation parameters");
            return;
        }

        qDebug() << "Emitting condTxCreateRequest";
        emit condTxCreateRequest(walletName, networkName, tokenName, value, unit);
    }

    // Order list request
    if (params->contains("search_orders"))
    {
        QJsonObject so = params->value("search_orders").toObject();
        QString networkName = so.value("network_name").toString();
        QString tokenName   = so.value("token_name").toString();
        QString unit        = so.value("unit").toString();
        QString maxPrice    = so.value("max_price").toString();
        QString minPrice    = so.value("min_price").toString();
        qDebug() << "search_orders command" << networkName << tokenName << unit << maxPrice << minPrice << so;

        qDebug() << "Emitting orderListRequest";
        emit orderListRequest(networkName, tokenName, minPrice, maxPrice, unit);
    }

    // Node detection check
    if (params->contains("node_detected_check") && params->value("node_detected_check").toBool()) {
        qDebug() << "Emitting sendNodeDetected";
        sendNodeDetected();
    }

    // Signature check
    if (params->contains("check_signed") && params->value("check_signed").toBool()) {
        qDebug() << "Emitting checkSigned";
        emit checkSigned();
    }

    // Start connection by order
    if (params->contains("start_connect_by_order"))
    {
        QJsonObject oi = params->value("start_connect_by_order").toObject();
        QString orderHash    = oi.value("hash").toString();
        QString srvUid       = oi.value("srvUid").toString();
        QString nodeAddress  = oi.value("nodeAddress").toString();
        QString network      = params->value ("network").toString();
        qDebug() << "start_connect_by_order" << oi;
        qDebug() << "Emitting connectByOrder";
        emit connectByOrder(srvUid, nodeAddress, orderHash, network);
    }

    // Get IP order list
    if (params->contains("get_ip_order_list"))
    {
        QJsonObject oi        = params->value("get_ip_order_list").toObject();
        QString srvUid        = oi.value("srv_uid").toString();
        QJsonArray orderList  = oi.value("node_adress_list").toArray();

        qDebug() << "get_ip_order_list - " << orderList;

        qDebug() << "Emitting getIpOrder";
        emit getIpOrder(srvUid, orderList);
    }

    // Request network fee data
    if (params->contains("get_fee_data"))
    {
        qDebug() << "Emitting getFeeData";
        emit getFeeData(params->value("get_fee_data").toString());
    }
}
