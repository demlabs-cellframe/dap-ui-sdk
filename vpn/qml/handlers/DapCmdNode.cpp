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

///**
// * @brief Class containing wallet data
// */
//class WalletsData
//{
//  /* DEFS */
//  struct Token
//  {
//    QString name;     // token name
//    QString balance;  // balanse
//    QString datoshi;  // datoshi price
//  };

//  struct Network
//  {
//    QString name;           // network name
//    QString address;        // network address
//    QStringList tokenNames; // fast access to token names
//    QList<Token> tokens;    // network tokens
//  };

//  struct Wallet
//  {
//    QString name;             // wallet name
//    QStringList networkNames; // fast access to network names
//    QStringList tokenNames;   // fast access to all token names
//    QList<Network> networks;  // wallet networks
//  };

//  /* VARS */

//  //QJsonObject m_walletsData;

//  QList<Wallet> _wallets;   // all wallets
//  QStringList _walletNames; // fast access to wallet names

//  /* METHODS */
//public:
//  /* parse data */
//  void setData (const QJsonObject &a_walletsData);

//  /* get wallets names list */
//  QStringList wallets();

//  /* get map with wallet names and corresponded tokens lists */
//  QHash<QString, QStringList> walletsWithTokens();

//  /* get wallet network names list */
//  QStringList networks (const QString &a_walletName);

//  /* get map with network names and corresponded tokens list */
//  QHash<QString, QStringList> networkWithTokens (const QString &a_walletName);

//  /* get map with token names and balances */
//  QHash<QString, QString> tokensAmount (const QString &a_walletName, const QString &network);

//protected:
//  void _parseWallets (const QJsonObject &a_data);
//  void _parseNetworks (const QJsonArray &a_list, Wallet &a_dest);
//  void _parseTokens (const QJsonArray &a_list, Network &a_dest, QStringList &a_tokenNames);
//};

/**
 * @brief DapCmdNode Data struct
 */
struct DapCmdNode::DapCmdNodeData
{
  bool hasError;
//  WalletsData dataWallet;
  OrderListData orderListData;

  QString selectedWalletName;
  QString selectedNetworkName;
  QString selectedTokenName;
  QString value;
  QString unit;
  QString maxPrice;
  QString minPrice;
  QString orderHash;
};

/* VARS */
static DapNodeTransactionHistory::Transaction s_historyOrder;
static QHash<QString, const char *> s_unitConvertMap =
{
  { "MEGABYTE", "mb" },
  { "KILOBYTE", "kb" },
  { "BYTE", "b" },
  { "SECOND", "sec" },
  { "DAY", "day" },
};

/* LINKS */
QDebug operator<< (QDebug dbg, const DapNodeOrderInfo &data);
static const char *convertUnits (const QString &a_value)
{
  return s_unitConvertMap.value (a_value, "");
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdNode::DapCmdNode (QObject *parent)
  : DapCmdClientAbstract (DapJsonCmdType::NODE_INFO, parent)
{
  nodeData  = new DapCmdNodeData();
  /* clear */
  s_historyOrder.isSigned = false;
}

DapCmdNode::~DapCmdNode()
{
  delete nodeData;
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdNode::handleResult (const QJsonObject &params)
{
  DEBUGINFO << __PRETTY_FUNCTION__ << params;
 nodeData->hasError = false;

//  // wallets list
//  if (params.value ("wallets").isArray())
//    {
//      auto jsonArray  = params.value ("wallets").toArray();
//      QStringList walletList;
//      for (const QJsonValue &value : qAsConst (jsonArray))
//        walletList << value.toString();

//      DEBUGINFO << "wallets" << walletList;
//      emit sigWalletsList (walletList);
//      return;
//    }

  // networks list
  if (params.value ("networks").isArray())
    {
//      QJsonArray array = params.value ("networks").toArray();
//      QMap<QString, QVariant> data;
//      foreach (const QVariant &vItem, array)
//        data[vItem.toString()] = "";
//      DEBUGINFO << "networks" << data;
////                    emit networksList(data);
      DEBUGINFO << "networks" << params.value ("networks");
      return;
    }

  // wallets data
  if (params.value ("wallets_data").isObject())
    {
      DEBUGINFO << "wallets_data" << params.value ("wallets_data");
      //nodeData->dataWallet.setData (params.value ("wallets_data").toObject());
      //emit sigWalletsList (nodeData->dataWallet.walletsWithTokens());
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

  if (params.value ("order_list").isArray())
    {
      DEBUGINFO << "order list";

      /* get list */
      auto list = params.value ("order_list").toArray();

      /* emit */
      emit sigOrderList (list);

      /* parse old style and emit */
      nodeData->orderListData.setData (list);
      //emit sigOrderListData (nodeData->orderListData.orders());
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
//    Q_UNUSED(code); Q_UNUSED(message);
//    qWarning() << *nodeData->errorObject;
//    switch (code) {
//    case 1:
//        emit sigResetSerialKeyError(message);
//        break;
//    case 2:
//        emit sigResetSerialKeyErrorSetOnlyMessage(message);
//        break;
//    }
  DEBUGINFO << __PRETTY_FUNCTION__ << QString ("%1 %2").arg (code).arg (message);
//    DEBUGINFO << "handleError" << message;
  QString errorMessage = message;
  int errorCode = code;
  emit sigNodeError (errorCode, errorMessage);
  return;

}

/********************************************
 * METHODS
 *******************************************/

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
  return nodeData->hasError;
}



DapNodeOrderInfo DapCmdNode::orderData (const QString &hash)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  return nodeData->orderListData.order (hash);
}

bool DapCmdNode::_checkContinue()
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  return  !nodeData->selectedWalletName.isEmpty() &&
          !nodeData->selectedNetworkName.isEmpty() &&
          !nodeData->selectedTokenName.isEmpty() &&
          !nodeData->value.isEmpty() &&
          !nodeData->orderHash.isEmpty();
}

void DapCmdNode::_updateHistoryItem()
{
  if (s_historyOrder.info.hash().isEmpty())
    return;

  auto &history = *DapNodeTransactionHistory::instance();
  int index     = history.indexOf (s_historyOrder.created);

  if (index == -1)
    return DapNodeTransactionHistory::instance()->prepend (s_historyOrder);

  history[index]  = s_historyOrder;
  history.itemUpdated (index);
  DapNodeTransactionHistory::instance()->setCurrentIndex (index);
}


/********************************************
 * SLOTS
 *******************************************/

void DapCmdNode::slotCondTxCreate()
{
  DEBUGINFO << __PRETTY_FUNCTION__;
//  QJsonObject condTx;
//  condTx["wallet_name"] = nodeData->selectedWalletName;
//  condTx["network_name"] = nodeData->selectedNetworkName;
//  condTx["token_name"] = nodeData->selectedTokenName;
////    condTx["cert_name"] = certName;
//  condTx["value"] = nodeData->value;
//  condTx["unit"] = "day"; // not used, filled with valid value

//  QJsonObject jObject;
//  jObject["cond_tx_create"] = condTx;

//  sendCmd (&jObject);

  auto order  = nodeData->orderListData.order (nodeData->orderHash);

  /* send command */
  QJsonObject jobj {
    { "cond_tx_create", QJsonObject {
        { "wallet_name",  nodeData->selectedWalletName },
        { "network_name", nodeData->selectedNetworkName },
        { "token_name",   nodeData->selectedTokenName },
        { "value",        nodeData->value },
        { "unit",         convertUnits (order.priceUnit()) },// "day" },
      },
    },
  };
  sendCmd (&jobj);

  /* store data */
  s_historyOrder.wallet   = nodeData->selectedWalletName;
  s_historyOrder.network  = nodeData->selectedNetworkName;
  s_historyOrder.token    = nodeData->selectedTokenName;
  s_historyOrder.value    = nodeData->value;
  s_historyOrder.unit     = order.priceUnit(); // "day";
  s_historyOrder.wallet   = nodeData->selectedWalletName;
  s_historyOrder.created  = QDateTime::currentDateTime();
  s_historyOrder.isSigned = false;
  _updateHistoryItem();
}

void DapCmdNode::slotStartSearchOrders()
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  QJsonObject searchOrders;
  qDebug() << "startSearchOrders" << nodeData->selectedNetworkName << nodeData->selectedTokenName << nodeData->unit << nodeData->minPrice << nodeData->maxPrice;
  searchOrders["network_name"] = nodeData->selectedNetworkName;
  searchOrders["token_name"] = nodeData->selectedTokenName;
  searchOrders["unit"] = nodeData->unit;
  searchOrders["min_price"] = nodeData->minPrice;
  searchOrders["max_price"] = nodeData->maxPrice;
  QJsonObject jObject;
  jObject["search_orders"] = searchOrders;
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
  auto order  = nodeData->orderListData.order (nodeData->orderHash);

  /* send command */
  QJsonObject jobj {
    { "start_connect_by_order", order.toJsonObject() },
    { "token", nodeData->selectedTokenName },
  };
  sendCmd (&jobj);

  /* store data */
  s_historyOrder.info = order;
  _updateHistoryItem();

//  QJsonObject jObject;
//  QJsonObject connectData = nodeData->orderListData.orderInfo (nodeData->orderHash);

//  connectData["token"] = nodeData->selectedTokenName;
//  //connectData["node_ip"] = "164.92.175.30"; // TODO get from order
//  jObject["start_connect_by_order"] = connectData;
  //  sendCmd (&jObject);
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
  nodeData->selectedWalletName = wallet;
  emit sigContinueEnable (_checkContinue());
//  emit sigNetworksList (nodeData->dataWallet.networkWithTokens (wallet));
}

void DapCmdNode::slotChooseNetwork (const QString &network)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->selectedNetworkName = network;
  emit sigContinueEnable (_checkContinue());
//  emit sigTokensInfo (nodeData->dataWallet.tokensAmount (nodeData->selectedWalletName, nodeData->selectedNetworkName));
}

void DapCmdNode::slotChooseToken (const QString &token)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->selectedTokenName = token;
//  auto tokens = nodeData->dataWallet.tokensAmount (nodeData->selectedWalletName, nodeData->selectedNetworkName);
  emit sigContinueEnable (_checkContinue());
//  emit sigTokenAmount (token, tokens[token]);
}

void DapCmdNode::slotSetValue (const QString &value)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->value = value;
  emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotSetUnit (const QString &value)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->unit = value;
  nodeData->orderListData.setUnit (nodeData->unit);
  emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotChooseOrder (const QString &hash)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->orderHash = hash;
  emit sigContinueEnable (_checkContinue());
}

void DapCmdNode::slotSetMaxValueUnit (const QString &price)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->maxPrice = price;
}

void DapCmdNode::slotSetMinValueUnit (const QString &price)
{
  DEBUGINFO << __PRETTY_FUNCTION__;
  nodeData->minPrice = price;
}

/*-----------------------------------------*/




/********************************************
 * METHODS
 *******************************************/

void OrderListData::setData (const QJsonArray &a_orderListData)
{
  for (const auto &jvalue : a_orderListData)
    {
      DapNodeOrderInfo order;
      order.fromJson (jvalue.toObject());
      m_orderMap.insert (order.hash(), order);
    }

//  m_orderListData = a_orderListData;
  _updateListMap();
}

QMap<QString, QVariant> &OrderListData::orders()
{
  return m_orders;
}

void OrderListData::_updateListMap()
{
//        foreach(const QJsonValue& item, nodeData->orderListData)
//        {
//            QJsonObject joItem = item.toObject();
//            QString key = joItem["hash"].toString().right(10);
//            QStringList list;
//            if (joItem.contains("node_location")
////                    && joItem.contains("node_addr")
////                    && !joItem["node_addr"].toString().isEmpty()
//               )
//            {
//                list.push_back(joItem["node_location"].toString());
//                list.push_back(QString("%1%2 per %3").arg(joItem["price"].toString()).arg(nodeData->unit).arg(joItem["price_unit"].toString()));
//                nodeData->orders[key].setValue(list);
//                qDebug() << joItem;
//            }
//        }

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

/*-----------------------------------------*/

//void WalletsData::setData (const QJsonObject &a_walletsData)
//{
//  //m_walletsData = a_walletsData;
//  _parseWallets (a_walletsData);
//}

//QStringList WalletsData::wallets()
//{
//  return _walletNames; // m_walletsData.keys();
//}

//QHash<QString, QStringList> WalletsData::walletsWithTokens()
//{
////  QMap<QString, QVariant> walletsWithTokenName;
////  foreach (const auto &walletName, m_walletsData.keys())
////    {
////      QSet<QString> aSet;
////      QJsonArray walletData = m_walletsData[walletName].toArray();
////      foreach (const auto &item, walletData)
////        foreach (const auto &token, item.toObject().value ("tokens").toArray())
////          aSet.insert (token.toObject().value ("tokenName").toString());
////      aSet.remove ("0");
////      walletsWithTokenName[walletName].setValue (aSet);
////    }
////  return walletsWithTokenName;

//  QHash<QString, QStringList> result;

//  for (auto i = _wallets.cbegin(), e = _wallets.cend(); i != e; i++)
//    result.insert (i->name, i->tokenNames);

//  return result;
//}

//QStringList WalletsData::networks (const QString &a_walletName)
//{
////  QStringList networks;
////  QJsonArray walletData = m_walletsData[wallet].toArray();
////  foreach (const auto &item, walletData)
////    networks.append (item.toObject().value ("network").toString());
////  return networks;

//  int index = _walletNames.indexOf (a_walletName);
//  if (index == -1)
//    return QStringList();
//  return _wallets.at (index).networkNames;
//}

//QHash<QString, QStringList> WalletsData::networkWithTokens (const QString &a_walletName)
//{
////  QMap<QString, QVariant> networkWithTokenName;
////  QJsonArray walletData = m_walletsData[walletName].toArray();
////  foreach (const auto &item, walletData)
////    {
////      QSet<QString> aSet;
////      QString networkName = item.toObject().value ("network").toString();
////      foreach (const auto &token, item.toObject().value ("tokens").toArray())
////        aSet.insert (token.toObject().value ("tokenName").toString());
////      aSet.remove ("0");
////      networkWithTokenName[networkName].setValue (aSet);
////    }
////  return networkWithTokenName;

//  /* vars */
//  QHash<QString, QStringList> result;

//  /* find wallet */
//  int index = _walletNames.indexOf (a_walletName);
//  if (index == -1)
//    return result;

//  /* get wallet */
//  const auto &wallet  = _wallets.at (index);

//  /* add all networks and their token names into result hash map */
//  for (auto i = wallet.networks.cbegin(), e = wallet.networks.cend(); i != e; i++)
//    result.insert (i->name, i->tokenNames);

//  return result;
//}

//QHash<QString, QString> WalletsData::tokensAmount (const QString &a_walletName, const QString &a_networkName)
//{
////  QMap<QString, QVariant> tokens;
////  QJsonArray walletData = m_walletsData[wallet].toArray();
////  foreach (const auto &item, walletData)
////    if (network == item.toObject().value ("network").toString())
////      foreach (const auto &item, item.toObject().value ("tokens").toArray())
////        tokens[item.toObject().value ("tokenName").toString()]
////          = item.toObject().value ("balance").toString();
////  if (tokens.contains ("0"))
////    tokens.remove ("0");
////  return tokens;

//  /* vars */
//  QHash<QString, QString> result;

//  /* find wallet */
//  int index = _walletNames.indexOf (a_walletName);
//  if (index == -1)
//    return result;

//  /* get wallet */
//  const auto &wallet  = _wallets.at (index);

//  /* find network */
//  index   = wallet.networkNames.indexOf (a_networkName);
//  if (index == -1)
//    return result;

//  /* get network */
//  const auto &network = wallet.networks.at (index);

//  /* collect data */
//  for (auto i = network.tokens.cbegin(), e = network.tokens.cend(); i != e; i++)
//    result.insert (i->name, i->balance);

//  return result;
//}

//void WalletsData::_parseWallets (const QJsonObject &a_data)
//{
//  _wallets.clear();
//  _walletNames.clear();

//  for (auto i = a_data.constBegin(), e = a_data.constEnd(); i != e; i++)
//  {
//    Wallet wallet;

//    /* fill with data */
//    wallet.name = i.key();
//    _parseNetworks (i.value().toArray(), wallet);

//    /* store name and data */
//    _walletNames.append (wallet.name);
//    _wallets.append (std::move (wallet));
//  }
//}

//void WalletsData::_parseNetworks (const QJsonArray &a_list, Wallet &a_dest)
//{
//  a_dest.networks.clear();
//  a_dest.networkNames.clear();

//  for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++)
//  {
//    Network network;
//    QJsonObject data  = i->toObject();

//    /* fill with data */
//    network.name    = data.value ("network").toString();
//    network.address = data.value ("address").toString();
//    _parseTokens (data.value ("tokens").toArray(), network, a_dest.tokenNames);

//    /* store name and data */
//    a_dest.networkNames.append (network.name);
//    a_dest.networks.append (std::move (network));
//  }
//}

//void WalletsData::_parseTokens (const QJsonArray &a_list, Network &a_dest, QStringList &a_tokenNames)
//{
//  a_dest.tokens.clear();
//  a_dest.tokenNames.clear();

//  for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++)
//  {
//    Token token;
//    QJsonObject data  = i->toObject();

//    /* fill with data */
//    token.name    = data.value ("tokenName").toString();
//    if (token.name == "0") // skip if empty
//      continue;
//    token.balance = data.value ("balance").toString();
//    token.datoshi = data.value ("datoshi").toString();

//    /* store name and data */
//    a_tokenNames.append (token.name);
//    a_dest.tokenNames.append (token.name);
//    a_dest.tokens.append (std::move (token));
//  }
//}

/*-----------------------------------------*/
