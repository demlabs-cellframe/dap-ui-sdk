#include "DapCmdNode.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

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
    DEBUGINFO << "slot setNodeDetected";
    m_nodeDetected = true;
    sendNodeDetected();
}

void DapCmdNode::sendNoCdbMode()
{
    QJsonObject response;
    response["nocdb_mode"] = m_nocdbMode;
    sendCmd(&response);
}

void DapCmdNode::sendNodeDetected()
{
    QJsonObject response;
    response["node_detected"] = m_nodeDetected;
    sendCmd(&response);
}

void DapCmdNode::sendError(int code, const QString& messageError)
{
    QJsonObject response;
    QJsonObject nodeInfo;
    nodeInfo["message"] = messageError;
    nodeInfo["code"] = code;
    response["error"] = nodeInfo;
    sendCmd(&response);
}

void DapCmdNode::sendWalletsList(const QStringList& walletsList)
{
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
    QJsonObject response;
    response["wallets_data"] = a_walletsData;
    sendCmd(&response);
    DEBUGINFO << "sendWalletsData";
}

void DapCmdNode::sendOrderList(const QJsonArray& orderList)
{
    QJsonObject response;
    response["order_list"] = orderList;
    sendCmd(&response);
    DEBUGINFO << "sendOrderList";
}

void DapCmdNode::sendTransactionInMempool()
{
    QJsonObject response;
    response["transaction_hash_in_mempool"] = true;
    sendCmd(&response);
    DEBUGINFO << "sendTransactionInMempool";
}

void DapCmdNode::sendTransactionInLedger()
{
    QJsonObject response;
    response["transaction_hash_in_ledger"] = true;
    sendCmd(&response);
    DEBUGINFO << "sendTransactionInLedger";
}

void DapCmdNode::sendSigningInfo(qint32 utype, qint64 uid, qint64 units, QString price)
{
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

void DapCmdNode::handle(const QJsonObject* params)
{
    if (params->value("start_node_detection").isBool() && params->value("start_node_detection").toBool())
        emit startNodeDetection();
    if (params->value("nocdb_mode_request").isBool())
        sendNoCdbMode();
    // data wallet request
    if (params->value("data_wallet_request").isBool() && params->value("data_wallet_request").toBool())
        emit dataWalletRequest();
    // stop checking the operation of the node
    if (params->value("start_node_detection").isBool() && !params->value("start_node_detection").toBool())
        emit stopNodeDetection();
    // creating a conditional transaction cmd
    if (params->value("cond_tx_create").isObject())
    {
        QJsonObject tx = params->value("cond_tx_create").toObject();
        QString walletName  = tx["wallet_name"].toString();
        QString networkName = tx["network_name"].toString();
        QString tokenName   = tx["token_name"].toString();
//            QString certName    = tx["cert_name"].toString();
        QString value         = tx["value"].toString();
        QString unit          = tx["unit"].toString();
        emit condTxCreateRequest(walletName, networkName, tokenName, value, unit);
    }
    // order list request
    if (params->value("search_orders").isObject())
    {
        QJsonObject so = params->value("search_orders").toObject();
        QString networkName = so["network_name"].toString();
        QString tokenName   = so["token_name"].toString();
        QString unit        = so["unit"].toString();
        QString maxPrice    = so["max_price"].toString();
        QString minPrice    = so["min_price"].toString();
        qDebug() << "nnnnnnnnnnnnnnnnnnnnnnnnnnn" << networkName << tokenName << unit << maxPrice << minPrice << so;
        emit orderListRequest(networkName, tokenName, minPrice, maxPrice, unit);
    }
    if (params->value("node_detected_check").isBool() && params->value("node_detected_check").toBool())
        sendNodeDetected();
    if (params->value("check_signed").isBool() && params->value("check_signed").toBool())
        emit checkSigned();
    if (params->value("start_connect_by_order").isObject())
    {
        QJsonObject oi = params->value("start_connect_by_order").toObject();
        //                == Order 0xD9A5C15D30A42615398AB7D3080FDEBCCD74FA3BB2E191F76EAC994326B45AA9 ==
        //                  version:          3
        //                  direction:        SERV_DIR_SELL
        //              V   srv_uid:          0x0000000000000001
        //                  price:            0.000000000000000002 (2)
        //                  price_unit:       DAY
        //                  node_addr:        58C0::CA70::6D11::1DCA
        //                  node_location:    Europe - Russia_2_1
        //              V   tx_cond_hash:     0x0000000000000000000000000000000000000000000000000000000000000000
        //                  ext:              0x52025275737369615F325F3100
        QString netId        = oi["net_id"].toString();
        QString txCondHash   = oi["tx_cond_hash"].toString();
        QString token        = oi["token"].toString();
        QString srvUid       = oi["srv_uid"].toString();
        QString address      = oi["node_ip"].toString();
//            QString address      = oi["node_addr"].toString();
        uint16_t port        = 80;
        if (!oi["port"].isNull())
            port             = oi["port"].toInt();
        m_nocdbMode = true;
        emit connectByOrder(netId, txCondHash, token, srvUid, address, port);
    }
}
