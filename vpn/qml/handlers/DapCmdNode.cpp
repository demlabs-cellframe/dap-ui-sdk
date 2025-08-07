/* INCLUDES */
#include "DapCmdNode.h"
#include "DapNodeTransactionHistory.h"
#include "DapNodeWalletData.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

// ui side

/* DEFS */
#define DEBUGINFO qDebug()<<" --->UiCMD<--- "

/**
 * @brief Class containind order list data
 */
class OrderListData
{
    /* VARS */
    DapNodeOrderInfoMap m_orderMap;
    QString m_unit;
    QMap<QString, QVariant> m_orders;

    /* METHODS */
public:
    void setData (const QJsonArray &a_orderListData);
    QMap<QString, QVariant> &orders();

    void setUnit (const QString &unit);

    DapNodeOrderInfo order (const QString &hash);
    QJsonObject orderInfo (const QString &hash);

private:
    void _updateListMap();
};

/**
 * @brief DapCmdNode Data struct
 */
struct DapCmdNode::DapCmdNodeData
{
    bool hasError;
    OrderListData orderListData;

    QString value;                // tokenValue
    QString selectedUnit;
    QString maxPrice;
    QString minPrice;
    QString orderHash;

    struct
    {
        QString ipAddress;
        QString network;
        QString wallet;
        QString token;
        QString tokenValue; // user payed
        QString unit;
        QString unitValue;
        QString priceValue; // per portion
        QString portions;

        QString totalValue;
    } overview;
};

/* VARS */
static DapNodeTransactionHistory::Transaction s_historyOrder;
static QHash<QString, const char *> s_unitConvertMap =
    {
        { "TERABYTE", "TB" },
        { "GIGABYTE", "GB" },
        { "MEGABYTE", "MB" },
        { "KILOBYTE", "KB" },
        { "BYTE",     "B" },
        { "SECOND",   "SEC" },
        { "MINUTE",   "MIN" },
        { "HOUR",     "HOUR" },
        { "DAY",      "DAY" },
        
        // Support for lowercase variants from orders
        { "sec",      "SEC" },
        { "min",      "MIN" },
        { "hour",     "HOUR" },
        { "day",      "DAY" },
        { "tb",       "TB" },
        { "gb",       "GB" },
        { "mb",       "MB" },
        { "kb",       "KB" },
        { "b",        "B" },
        };

/* LINKS */
QDebug operator<< (QDebug dbg, const DapNodeOrderInfo &data);
static const char *convertUnits (const QString &a_value)
{
    return s_unitConvertMap.value (a_value, "");
}

DapCmdNode::DapCmdNode (QObject *parent)
    : DapCmdClientAbstract (DapJsonCmdType::NODE_INFO, parent)
    , _data (new DapCmdNodeData)
{
    /* clear */
    s_historyOrder.isSigned = false;
}

DapCmdNode::~DapCmdNode()
{
    delete _data;
}

void DapCmdNode::handleResult (const QJsonObject &params)
{
    DEBUGINFO << __PRETTY_FUNCTION__ << params;
    _data->hasError = false;

    // networks list
    if (params.value ("networks").isArray())
    {
        DEBUGINFO << "networks" << params.value ("networks");
        return;
    }

    // wallets data
    if (params.value ("wallets_data").isObject())
    {
        DEBUGINFO << "wallets_data" << params.value ("wallets_data");
        DapNodeWalletData::instance()->setWalletsData (params.value ("wallets_data").toObject());
        emit sigWalletsDataUpdated();
        return;
    }

    // node detected status
    if (params.value ("node_detected").isBool() && params.value ("node_detected").toBool())
    {
        DEBUGINFO << "node detected";
        emit sigNodeDetected();
        return;
    }

    // transaction hash in mempool
    if (params.value ("transaction_hash_in_mempool").isBool() && params.value ("transaction_hash_in_mempool").toBool())
    {
        DEBUGINFO << "transaction hash in mempool";
        emit sigTransactionHashInMempool();
        return;
    }

    // transaction hash in ledger
    if (params.value ("transaction_hash_in_ledger").isBool() && params.value ("transaction_hash_in_ledger").toBool())
    {
        DEBUGINFO << "transaction hash in ledger";
        emit sigTransactionHashInledger();
        return;
    }

    // transaction in queue
    if (params.value ("transaction_in_queue").isBool() && params.value ("transaction_in_queue").toBool())
    {
        QString queueId = params.value ("queue_id").toString();
        QString appType = params.value ("app_type").toString();
        DEBUGINFO << "transaction in queue with ID:" << queueId << "App type:" << appType;
        emit sigTransactionInQueue(queueId, appType);
        return;
    }

    if (params.value ("order_list").isArray())
    {
        DEBUGINFO << "order list";

        /* get list */
        auto list = params.value ("order_list").toArray();
        qDebug() << "🔍 [FILTER DEBUG] DapCmdNode::handleResult: Received order_list count =" << list.size();

        /* emit */
        emit sigOrderList (list);

        /* parse old style and emit */
        _data->orderListData.setData (list);
        emit sigOrderListDataUpdated();

        return;
    }

    if (params.value ("signing_info").isObject())
    {
        QJsonObject info = params.value ("signing_info").toObject();
        qint32 utype = info.value ("utype").toInt();
        qint64 uid = info.value ("uid").toInt();
        QString units = info.value ("units").toString();
        QString value = info.value ("value").toString();
        DEBUGINFO << "signing_info received" << units << value;
        emit sigSigningReceived (utype, uid, units, value);
        return;
    }

    if (params.value ("node_ip_list").isObject())
    {
        auto jobj  = params.value ("node_ip_list").toObject();
        emit sigNodeIpList (jobj);
        return;
    }

    if (params.value ("get_fee_data").isObject())
    {
        auto jobj  = params.value ("get_fee_data").toObject();
        emit sigFeeData (jobj);
        return;
    }
}

void DapCmdNode::handleError (int code, const QString &message)
{
    _data->hasError = true;
    DEBUGINFO << __PRETTY_FUNCTION__ << QString ("%1 %2").arg (code).arg (message);
    //    DEBUGINFO << "handleError" << message;
    QString errorMessage = message;
    int errorCode = code;
    emit sigNodeError (errorCode, errorMessage);
    return;

}

void DapCmdNode::startNodeDetection()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["start_node_detection"] = true;
    sendCmd (&checkNode);
}

void DapCmdNode::stopCheckNode()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["start_node_detection"] = false;
    sendCmd (&checkNode);
}

void DapCmdNode::noCdbModeRequest()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["nocdb_mode_request"] = true;
    sendCmd (&checkNode);
}

void DapCmdNode::walletsRequest()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    QJsonObject checkNode;
    checkNode["data_wallet_request"] = true;
    sendCmd (&checkNode);
}

bool DapCmdNode::hasError()
{
    return _data->hasError;
}

DapNodeOrderInfo DapCmdNode::orderData (const QString &hash)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    return _data->orderListData.order (hash);
}

void DapCmdNode::convertUnits (QString &a_unit, qint64 &a_min, qint64 &a_max, qint64 *a_multiplier)
{
    /* defines */

    struct MultiplierItem
    {
        qint64 value;
        QString unit;
    };

    /* variables */

    static const QMap<QString, MultiplierItem> s_multiplierMap =
        {
         { "Minute",   MultiplierItem { 60, "Second" } },
         { "Hour",     MultiplierItem { 60 * 60, "Second" } },
         { "Day",      MultiplierItem { 24 * 60 * 60, "Second" } },

         { "Kilobyte", MultiplierItem { qint64 (1024), "Byte" } },
         { "Megabyte", MultiplierItem { qint64 (1024) * qint64 (1024), "Byte" } },
         { "Gigabyte", MultiplierItem { qint64 (1024) * qint64 (1024) * qint64 (1024), "Byte" } },
         { "Terabyte", MultiplierItem { qint64 (1024) * qint64 (1024) * qint64 (1024) * qint64 (1024), "Byte" } },
         };

    QString unit      = a_unit;
    qint64 minPrice   = a_min;
    qint64 maxPrice   = a_max;
    MultiplierItem mi = s_multiplierMap.value (unit);

    /* convert values, if unit is present */

    if (!mi.unit.isEmpty())
    {
        DEBUGINFO << __PRETTY_FUNCTION__ << "multiplier:" << mi.value << ", unit:" << mi.unit;

        if (minPrice > 0)
            minPrice  = minPrice * mi.value;

        if (maxPrice > 0)
            maxPrice  = maxPrice * mi.value;

        unit      = mi.unit;
    }

    /* return */

    a_unit  = unit;
    a_min   = minPrice;
    a_max   = maxPrice;

    if (a_multiplier)
        *a_multiplier = mi.value;
}

bool DapCmdNode::_checkContinue()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    return  !_data->overview.wallet.isEmpty() &&
           !_data->overview.network.isEmpty() &&
           !_data->overview.token.isEmpty() &&
           !_data->value.isEmpty() &&
           !_data->orderHash.isEmpty();
}

void DapCmdNode::_updateHistoryItem()
{
    if (s_historyOrder.info.hash().isEmpty())
        return;

    auto &history = *DapNodeTransactionHistory::instance();
    int index     = history.indexOf (s_historyOrder.info.hash());

    if (index == -1)
        return DapNodeTransactionHistory::instance()->append (s_historyOrder);

    history[index]  = s_historyOrder;
    history.itemUpdated (index);
    DapNodeTransactionHistory::instance()->setCurrentIndex (index);
}

void DapCmdNode::slotCondTxCreate()
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    auto order  = _data->orderListData.order (_data->orderHash);

    /* send command */
    QJsonObject jobj {
                     { "cond_tx_create", QJsonObject {
                                     { "wallet_name",  _data->overview.wallet },
                                     { "network_name", _data->overview.network },
                                     { "token_name",   _data->overview.token },
                                     { "value",        _data->value },
                                     { "unit",         ::convertUnits (order.priceUnit()) },// "day" },
                                     },
                         },
                     };
    sendCmd (&jobj);

    /* store data */
    s_historyOrder.ipAddress    = _data->overview.ipAddress;
    s_historyOrder.network      = _data->overview.network;
    s_historyOrder.wallet       = _data->overview.wallet;
    s_historyOrder.token        = _data->overview.token;
    s_historyOrder.tokenValue   = _data->overview.tokenValue;
    s_historyOrder.unit         = _data->overview.unit; // order.priceUnit(); // "day";
    s_historyOrder.unitValue    = _data->overview.unitValue; // order.price();
    s_historyOrder.priceValue   = _data->overview.priceValue;
    s_historyOrder.portions     = _data->overview.portions;

    s_historyOrder.totalValue   = _data->overview.totalValue;
    s_historyOrder.created      = QDateTime::currentDateTime();
    s_historyOrder.isSigned     = false;
    _updateHistoryItem();
}

void DapCmdNode::slotStartSearchOrders()
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    /* variables */

    QString unit      = _data->selectedUnit;
    qint64 minPrice   = _data->minPrice.toULongLong();
    qint64 maxPrice   = _data->maxPrice.toULongLong();

    /* convert units */

    convertUnits (unit, minPrice, maxPrice);

    /* filter data */

    QJsonObject searchOrders;

    qDebug() << "startSearchOrders"
             << _data->overview.network
             << _data->overview.token
             << _data->overview.unit
             << _data->minPrice
             << _data->maxPrice;

    searchOrders["network_name"]  = _data->overview.network;
    searchOrders["token_name"]    = _data->overview.token;
    searchOrders["unit"]          = _data->selectedUnit;
    searchOrders["min_price"]     = _data->minPrice;
    searchOrders["max_price"]     = _data->maxPrice;

    /* message body */

    QJsonObject jObject;
    jObject["search_orders"]      = searchOrders;
    sendCmd (&jObject);
}

void DapCmdNode::slotCheckSigned()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    qDebug() << "check signed";
    QJsonObject checkNode;
    checkNode["check_signed"] = true;
    sendCmd (&checkNode);

    /* store into history */
    s_historyOrder.isSigned = true;
    _updateHistoryItem();
}

void DapCmdNode::slotStartConnectByOrder()
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    /* get order */
    auto order  = _data->orderListData.order (_data->orderHash);

    /* send command */
    QJsonObject jobj {
                     { "start_connect_by_order", order.toJsonObject() },
                     { "token", _data->overview.token },
                     };
    sendCmd (&jobj);

    /* store data */
    s_historyOrder.info = order;
    _updateHistoryItem();
}

void DapCmdNode::slotStartConnectByHistoryOrder (const DapNodeOrderInfo &a_info, const QString &a_token, const QString &a_network)
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    /* send command */
    QJsonObject jobj {
        { "start_connect_by_order", a_info.toJsonObject() },
        { "token", a_token },
        { "network", a_network }
    };
    sendCmd (&jobj);
}

void DapCmdNode::slotRequestIpNode (const QString &networkName, const QJsonArray &orderList)
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    QJsonObject jobj {
                     { "srv_uid",          networkName },
                     { "node_adress_list", orderList },
                     };

    QJsonObject request {
        { "get_ip_order_list", jobj }
    };

    sendCmd (&request);
}

void DapCmdNode::slotRequestNetworkFee (const QString &a_networkName)
{
    DEBUGINFO << __PRETTY_FUNCTION__ << a_networkName;
    QJsonObject request {
        { "get_fee_data", a_networkName }
    };
    sendCmd (&request);
}

void DapCmdNode::slotChooseWallet (const QString &wallet)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->overview.wallet = wallet;
    emit sigContinueEnable (_checkContinue());
    //  emit sigNetworksList (_data->dataWallet.networkWithTokens (wallet));
}

void DapCmdNode::slotChooseNetwork (const QString &network)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->overview.network = network;
    emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotChooseToken (const QString &token)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->overview.token = token;
    emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotSetValue (const QString &value)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->value = value;
    emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotSetUnit (const QString &value)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->selectedUnit = value;
    _data->orderListData.setUnit (_data->selectedUnit);
    emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotChooseOrder (const QString &hash)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->orderHash = hash;
    emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotSetTransactionInfo (const QVariant &a_valueMap)
{
    DEBUGINFO << __PRETTY_FUNCTION__;

    /* defines */

    enum class FieldId
    {
        INVALID,

        ipAddress,
        network,
        wallet,
        token,
        tokenValue,
        unit,
        unitValue,
        priceValue,
        portions,

        totalValue,
    };

#define TFIELDITEM(n) { #n, FieldId::n }

    /* variables */

    static const QMap<QString, FieldId> fieldMap =
        {
            TFIELDITEM (ipAddress),
            TFIELDITEM (network),
            TFIELDITEM (wallet),
            TFIELDITEM (token),
            TFIELDITEM (tokenValue),
            TFIELDITEM (unit),
            TFIELDITEM (unitValue),
            TFIELDITEM (priceValue),
            TFIELDITEM (portions),

            TFIELDITEM (totalValue),
        };
    const QVariantMap map = a_valueMap.toMap();

    /* parse */

    for (auto i = map.cbegin(), e = map.cend(); i != e; i++)
    {
        FieldId fid = fieldMap.value (i.key(), FieldId::INVALID);

        if (fid == FieldId::INVALID)
        {
            DEBUGINFO << __PRETTY_FUNCTION__ << "invalid fid:" << i.key();
            continue;
        }

        auto value  = i.value().toString();

        switch (fid)
        {
        case FieldId::ipAddress:  _data->overview.ipAddress   = std::move (value); break;
        case FieldId::network:    _data->overview.network     = std::move (value); break;
        case FieldId::wallet:     _data->overview.wallet      = std::move (value); break;
        case FieldId::token:      _data->overview.token       = std::move (value); break;
        case FieldId::tokenValue: _data->overview.tokenValue  = std::move (value); break;
        case FieldId::unit:       _data->overview.unit        = std::move (value); break;
        case FieldId::unitValue:  _data->overview.unitValue   = std::move (value); break;
        case FieldId::priceValue: _data->overview.priceValue  = std::move (value); break;
        case FieldId::portions:   _data->overview.portions    = std::move (value); break;

        case FieldId::totalValue: _data->overview.totalValue  = std::move (value); break;
        default:
            DEBUGINFO << __PRETTY_FUNCTION__ << "unknown fid:" << uint (fid) << i.key();
            break;
        }
    }
}

void DapCmdNode::slotSetMaxValueUnit (const QString &price)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->maxPrice = price;
}

void DapCmdNode::slotSetMinValueUnit (const QString &price)
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    _data->minPrice = price;
}

void OrderListData::setData (const QJsonArray &a_orderListData)
{
    for (const auto &jvalue : a_orderListData)
    {
        DapNodeOrderInfo order;
        order.fromJson (jvalue.toObject());
        m_orderMap.insert (order.hash(), order);
    }

    _updateListMap();
}

QMap<QString, QVariant> &OrderListData::orders()
{
    return m_orders;
}

void OrderListData::_updateListMap()
{
    for (const auto &item : qAsConst (m_orderMap))
    {
        QString key     = item.hash();//.right (10);
        QString loc     = item.nodeLocation();

        QString result  = QString ("%1%2 per %3")
                             .arg (item.price(), m_unit, item.priceUnit());

        m_orders[key].setValue (QStringList {std::move (loc), std::move (result)});

        qDebug() << item;
    }
}

void OrderListData::setUnit (const QString &unit)
{
    m_unit = unit == "All"
                 ? QString()
                 : unit;
}

DapNodeOrderInfo OrderListData::order (const QString &hash)
{
    return m_orderMap.value (hash, DapNodeOrderInfo());
}

QJsonObject OrderListData::orderInfo (const QString &hash)
{
    return m_orderMap.value (hash, DapNodeOrderInfo()).toJsonObject();
    //                == Order 0xD9A5C15D30A42615398AB7D3080FDEBCCD74FA3BB2E191F76EAC994326B45AA9 ==
    //                  version:          3
    //                  direction:        SERV_DIR_SELL
    //                  srv_uid:          0x0000000000000001
    //                  price:            0.000000000000000002 (2)
    //                  price_unit:       DAY
    //                  node_addr:        58C0::CA70::6D11::1DCA
    //                  node_location:    Europe - Russia_2_1
    //                  tx_cond_hash:     0x0000000000000000000000000000000000000000000000000000000000000000
    //                  ext:              0x52025275737369615F325F3100
}

QString DapCmdNode::getConnectedAppType() const
{
    return m_connectedAppType;
}

void DapCmdNode::setConnectedAppType(const QString& appType)
{
    if (m_connectedAppType != appType) {
        m_connectedAppType = appType;
        qDebug() << "[DapCmdNode] Connected app type updated to:" << appType;
    }
}

void DapCmdNode::slotWalletsDataRequest()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    walletsRequest();
}

void DapCmdNode::slotNodeDetection()
{
    DEBUGINFO << __PRETTY_FUNCTION__;
    startNodeDetection();
}
