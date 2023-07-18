
#include "DapCmdNode.h"
#include <QDebug>

#define DEBUGINFO qDebug()<<" --->UiCMD<--- "

// ui side


void DapCmdNode::handleResult(const QJsonObject& params)
{
    DEBUGINFO << __PRETTY_FUNCTION__ << " status OK";
    m_hasError = false;
    // wallets list
    if (params.value("wallets").isArray())
    {
        QJsonArray array = params.value("wallets").toArray();
        QMap<QString, QVariant> data;
        foreach(const QVariant& vItem, array)
            data[vItem.toString()] = "";
        DEBUGINFO << "wallets" << data;
//                    emit walletsList(data);
        return;
    }
    // networks list
    if (params.value("networks").isArray())
    {
        QJsonArray array = params.value("networks").toArray();
        QMap<QString, QVariant> data;
        foreach(const QVariant& vItem, array)
            data[vItem.toString()] = "";
        DEBUGINFO << "networks" << data;
//                    emit networksList(data);
        return;
    }
    // wallets data
    if (params.value("wallets_data").isObject())
    {
        DEBUGINFO << "wallets_data" << params.value("wallets_data");
        m_dataWallet.setData(params.value("wallets_data").toObject());
        emit walletsList(m_dataWallet.walletsWithTokens());
        return;
    }
    // node detected status
    if (params.value("node_detected").isBool() && params.value("node_detected").toBool())
    {
        DEBUGINFO << "node detected";
        emit nodeDetected();
        return;
    }
    // transaction hash in mempool
    if (params.value("transaction_hash_in_mempool").isBool() && params.value("transaction_hash_in_mempool").toBool())
    {
        DEBUGINFO << "transaction hash in mempool";
        emit transactionHashInMempool();
        return;
    }
    // transaction hash in ledger
    if (params.value("transaction_hash_in_ledger").isBool() && params.value("transaction_hash_in_ledger").toBool())
    {
        DEBUGINFO << "transaction hash in ledger";
        emit transactionHashInledger();
        return;
    }
    if (params.value("order_list").isArray())
    {
        DEBUGINFO << "order list";
        m_orderListData.setData(params.value("order_list").toArray());
        emit orderList(m_orderListData.orders());
        return;
    }
    if (params.value("signing_info").isObject())
    {
        QJsonObject info = params.value("signing_info").toObject();
        qint32 utype = info.value("utype").toInt();
        qint64 uid = info.value("uid").toInt();
        QString units = info.value("units").toString();
        QString value = info.value("value").toString();
        DEBUGINFO << "signing_info received" << units << value;
        emit signingReceived(utype, uid, units, value);
        return;
    }
}

void DapCmdNode::handleError(int code, const QString& message)
{
//    Q_UNUSED(code); Q_UNUSED(message);
//    qWarning() << *m_errorObject;
//    switch (code) {
//    case 1:
//        emit sigResetSerialKeyError(message);
//        break;
//    case 2:
//        emit sigResetSerialKeyErrorSetOnlyMessage(message);
//        break;
//    }
    m_hasError = true;
    DEBUGINFO << __PRETTY_FUNCTION__ << " " + message;
//    DEBUGINFO << "handleError" << message;
    QString errorMessage = message;
    int errorCode = code;
    emit nodeError(errorCode, errorMessage);
    return;

}

void DapCmdNode::startNodeDetection()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["start_node_detection"] = true;
    sendCmd(&checkNode);
}

void DapCmdNode::stopCheckNode()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["start_node_detection"] = false;
    sendCmd(&checkNode);
}

void DapCmdNode::noCdbModeRequest()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["nocdb_mode_request"] = true;
    sendCmd(&checkNode);
}

void DapCmdNode::condTxCreate()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject condTx;
    condTx["wallet_name"] = m_selectedWalletName;
    condTx["network_name"] = m_selectedNetworkName;
    condTx["token_name"] = m_selectedTokenName;
//    condTx["cert_name"] = certName;
    condTx["value"] = m_value;
    condTx["unit"] = "day"; // not used, filled with valid value
    QJsonObject jObject;
    jObject["cond_tx_create"] = condTx;
    sendCmd(&jObject);
}

void DapCmdNode::startSearchOrders()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject searchOrders;
    qDebug() << "startSearchOrders" << m_selectedNetworkName << m_selectedTokenName << m_unit << m_minPrice << m_maxPrice;
    searchOrders["network_name"] = m_selectedNetworkName;
    searchOrders["token_name"] = m_selectedTokenName;
    searchOrders["unit"] = m_unit;
    searchOrders["min_price"] = m_minPrice;
    searchOrders["max_price"] = m_maxPrice;
    QJsonObject jObject;
    jObject["search_orders"] = searchOrders;
    sendCmd(&jObject);
}

void DapCmdNode::checkSigned()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    qDebug() << "check signed";
    QJsonObject checkNode;
    checkNode["check_signed"] = true;
    sendCmd(&checkNode);
}

void DapCmdNode::startConnectByOrder()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject jObject;
    QJsonObject connectData = m_orderListData.orderInfo(m_orderHash);
    connectData["token"] = m_selectedTokenName;
    //connectData["node_ip"] = "164.92.175.30"; // TODO get from order
    jObject["start_connect_by_order"] = connectData;
    sendCmd(&jObject);
}

void DapCmdNode::chooseWallet(QString wallet)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_selectedWalletName = wallet;
    emit continueEnable(checkContinue());
    emit networksList(nodeInfo()->networkWithTokens(wallet));
}

void DapCmdNode::chooseNetwork(QString network)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_selectedNetworkName = network;
    emit continueEnable(checkContinue());
    emit tokensInfo(nodeInfo()->tokensAmount(m_selectedWalletName, m_selectedNetworkName));
}

void DapCmdNode::chooseToken(QString token)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_selectedTokenName = token;
    auto tokens = nodeInfo()->tokensAmount(m_selectedWalletName, m_selectedNetworkName);
    emit continueEnable(checkContinue());
    emit tokenAmount(token, tokens[token].toString());
}

void DapCmdNode::setValue(QString value)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_value = value;
    emit continueEnable(checkContinue());
}

void DapCmdNode::setUnit(QString value)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_unit = value;
    m_orderListData.setUnit(m_unit);
    emit continueEnable(checkContinue());
}

QStringList DapCmdNode::orderData(QString hash)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    return m_orderListData.order(hash);
}

void DapCmdNode::chooseOrder(QString hash)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_orderHash = hash;
    emit continueEnable(checkContinue());
}

void DapCmdNode::setMaxValueUnit(QString price)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_maxPrice = price;
}

void DapCmdNode::setMinValueUnit(QString price)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    m_minPrice = price;
}

bool DapCmdNode::checkContinue()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    return  !m_selectedWalletName.isEmpty() &&
            !m_selectedNetworkName.isEmpty() &&
            !m_selectedTokenName.isEmpty() &&
            !m_value.isEmpty() &&
            !m_orderHash.isEmpty();
}
