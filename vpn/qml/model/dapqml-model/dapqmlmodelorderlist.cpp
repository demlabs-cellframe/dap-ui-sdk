/* INCLUDES */
#include "dapqmlmodelorderlist.h"
#include "modules/orderlistmodules.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/********************************************
 * DEFS
 *******************************************/

//#define ENABLE_ORDERLIST_SIMULATION
//#define ENABLE_NAME_VALUE_DUMMY
#define DEBUG_MSG qDebug() << "DapQmlModelOrderList" << __func__

typedef DapQmlModelOrderList::FieldId FieldId;
using namespace OrderListModule;

/*-----------------------------------------*/

struct DapQmlModelOrderList::DapQmlModelOrderListData
{
  ModuleContainer module;
  QString network;
  QString wallet;
  QString token;
  QString unit;
};

/********************************************
 * VARIABLES
 *******************************************/

static QHash<int, QByteArray> s_fields =
{
  { int (FieldId::location),    "location" },
  { int (FieldId::node_addr),   "node_addr" },
  { int (FieldId::price),       "price" },
  { int (FieldId::priceShort),  "priceShort" },
  { int (FieldId::units),       "units" },
  { int (FieldId::units_value), "units_value" },
  { int (FieldId::hash),        "hash" },
  { int (FieldId::ipAddress),   "ipAddress" },

  { int (FieldId::name),        "name" },
  { int (FieldId::value),       "value" },

  { int (FieldId::network),     "network" },
  { int (FieldId::server),      "server" },
  { int (FieldId::wallet),      "wallet" },
};

static DapQmlModelOrderList *_instance = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderList::DapQmlModelOrderList (QObject *parent)
  : QAbstractListModel {parent}
  , _data (new DapQmlModelOrderListData)
{
#ifdef ENABLE_ORDERLIST_SIMULATION
  auto result = QJsonDocument::fromJson ("[\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024672616E636500\",\n            \"hash\": \"0xF84FC4D96D564E2A54DC6D82A8F90682F0276CB5EF3B26A8A4BF3C18E39EE9F4\",\n            \"node_addr\": \"D860::D9D5::1C57::A6B3\",\n            \"node_location\": \"Europe-France\",\n            \"pkey\": \"0xC6FB9B9370C01F52AD31D1B22A8AB1F1B18AF190EB48BA8F7E24E3DA6E67C8C2\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5202554B00\",\n            \"hash\": \"0x243BBFBBB7C4A360646567A88066F93815E77EB2ECD4B480AF96F7E2B1996E9D\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Europe-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5206554B00\",\n            \"hash\": \"0x3DB2E19637239001931F2F84F17665164800633E5D263A404ED0264A5CE4F07C\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Asia-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.000000000000000002(2)\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024765726D616E7900\",\n            \"hash\": \"0xF4590296D5844C301FCDA29F7854ADE6FFA2A56044BC1D4995A548342987F09A\",\n            \"node_addr\": \"E02A::FB56::1B0A::19C8\",\n            \"node_location\": \"Europe-Germany\",\n            \"pkey\": \"0xE23EF5A0EA0604EADD74EB2F365809AE222EB88B7EA72C9ADA7AF8B78EA4E21E\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"600\",\n            \"version\": \"3\"\n        }\n    ]\n").array();
  slotSetOrderListData (result);
  setServerName ("Server name");
  setNetwork ("TestNetwork");
  setWallet ("TestWallet");
#endif // ENABLE_ORDERLIST_SIMULATION

#ifdef ENABLE_NAME_VALUE_DUMMY
  try
    {
      QVector<NameValueItem> items =
      {
        NameValueItem{ "name1", "value1" },
        NameValueItem{ "name2", "value2" },
        NameValueItem{ "name3", "value3" },
      };
      s_modules.networks->as<NetworksModule>()->setItems (items);
      s_modules.wallets->as<WalletsModule>()->setItems (items);
      s_modules.tokens->as<TokensModule>()->setItems (items);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }
#endif // ENABLE_NAME_VALUE_DUMMY
}

DapQmlModelOrderList::~DapQmlModelOrderList()
{
  delete _data;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelOrderList *DapQmlModelOrderList::instance()
{
  if (_instance == nullptr)
    _instance = new DapQmlModelOrderList;
  return _instance;
}

QObject *DapQmlModelOrderList::singletonProvider (QQmlEngine *, QJSEngine *)
{
  return instance();
}

int DapQmlModelOrderList::length() const
{
  try
    {
      return _data->module->size();
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return -1;
    }
}

int DapQmlModelOrderList::currentIndex() const
{
  try
    {
      return _data->module->currentIndex();
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return -1;
    }
}

void DapQmlModelOrderList::setCurrentIndex (int a_index)
{
  try
    {
      if (!_data->module->setCurrentIndex (a_index))
        return;
      emit sigCurrentIndexChanged();
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }
}

int DapQmlModelOrderList::indexOf (const QString &a_location)
{
  try
    {
      /* get module order list */
      auto list = _data->module->as<OrdersModule>()->items();

      /* iterate in cycle */
      int index = 0;
      for (auto i = list.cbegin(), e = list.cend(); i != e; i++, index++)
        if (i->location == a_location)
          return index;

      return -1;
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return -1;
    }
}

DapQmlModelOrderList::Mode DapQmlModelOrderList::mode() const
{
  return _data->module.mode();
}

void DapQmlModelOrderList::setMode (Mode a_value)
{
  /* notify model */
  beginResetModel();

  if (!_data->module.setMode (a_value))
    return;

  /* notify model */
  endResetModel();

  emit sigModeChanged();
}

QString DapQmlModelOrderList::network() const
{
  return _data->network;
}

void DapQmlModelOrderList::setNetwork (const QString &a_value)
{

  if (a_value.isEmpty())
    _data->network  = _data->module.networks()->name();
  else
    {
      if (_data->network == a_value)
        return;
      else
        _data->network = a_value;
    }

  emit sigNetworkChanged();
}

QString DapQmlModelOrderList::wallet() const
{
  return _data->wallet;
}

void DapQmlModelOrderList::setWallet (const QString &a_value)
{
  if (a_value.isEmpty())
    _data->wallet  = _data->module.wallets()->name();
  else
    {
      if (_data->wallet == a_value)
        return;
      else
        _data->wallet = a_value;
    }

  emit sigWalletChanged();
}

QString DapQmlModelOrderList::token() const
{
  return _data->token;
}

void DapQmlModelOrderList::setToken (const QString &a_value)
{
  if (a_value.isEmpty())
    _data->token  = _data->module.tokens()->name();
  else
    {
      if (_data->token == a_value)
        return;
      else
        _data->token = a_value;
    }

  emit sigTokenChanged();
}

QString DapQmlModelOrderList::unit() const
{
  return _data->unit;
}

void DapQmlModelOrderList::setUnit (const QString &a_value)
{
  if (a_value.isEmpty())
    _data->unit  = _data->module.units()->name();
  else
    {
      if (_data->unit == a_value)
        return;
      else
        _data->unit = a_value;
    }

  emit sigUnitChanged();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelOrderList::rowCount (const QModelIndex &) const
{
  return length();
}

int DapQmlModelOrderList::columnCount (const QModelIndex &) const
{
  return s_fields.size();
}

QVariant DapQmlModelOrderList::data (const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch (FieldId (role))
    {
    case FieldId::network:    return _data->network;
    case FieldId::wallet:     return _data->wallet;
    default:
      return _data->module->data (index, role);
    }
}

QHash<int, QByteArray> DapQmlModelOrderList::roleNames() const
{
  return s_fields;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelOrderList::slotSetOrderListData (const QJsonArray &a_list)
{
//  if (a_list.isEmpty())
//    return;

  /* notify model */
  beginResetModel();

  /* vars */
  QVector<OrderItem> items;
  QSet<QString> addressesSet;
  QJsonArray jarray;

  /* parse via cycle */
  for (const auto &item : qAsConst (a_list))
    {
      /* get item */
      QJsonObject joItem = item.toObject();
      if (!joItem.contains ("node_location"))
        continue;

      /* get fields */
      QString loc        = joItem.value ("node_location").toString();
      QString price      = joItem.value ("price").toString();
      QString punit      = joItem.value ("price_unit").toString();
      QString unit_value = joItem.value ("units").toString();
      QString server     = joItem.value ("node_location").toString();
      QString node_addr  = joItem.value ("node_addr").toString();
      QString hash       = joItem.value ("hash").toString();
      QString srv_uid    = joItem.value ("srv_uid").toString();

      /* check */
      if (srv_uid != "0x0000000000000001"
          || unit_value == "0"
          || punit.isEmpty())
        continue;

      /* store result */
      addressesSet << node_addr;
      items << OrderItem
      {
        std::move (loc),
        std::move (price),
        std::move (punit),
        std::move (unit_value),
        std::move (server),
        std::move (node_addr),
        std::move (hash),
        QString()
      };
    }

  /* store result */
  try
    {
      auto orders = _data->module.orders()->as<OrdersModule>();
      orders->setItems (std::move (items));
      orders->setCurrentIndex (0);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();

  /* collect addresses into json */
  for (const auto &address : qAsConst (addressesSet))
    jarray << address;

  /* request ip's */
  emit sigRequestNodeIPs (network(), jarray);
}

void DapQmlModelOrderList::slotSetWalletListData (const QHash<QString, QStringList> &a_walletData)
{
  /* notify model */
  beginResetModel();

  /* vars */
  QVector<NameValueItem> items;

  /* parse via cycle */
  for (auto i = a_walletData.cbegin(), e = a_walletData.cend(); i != e; i++)
    {
      auto list = i.value();
      items.append (NameValueItem
      {
        i.key(),
        (list.isEmpty()) ? QString() : list.first()
      });
    }

  /* store result */
  try
    {
      auto wallets  = _data->module.wallets()->as<WalletsModule>();
      wallets->setItems (std::move (items));
      wallets->setCurrentIndex (0);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();
}

void DapQmlModelOrderList::slotSetNetworkListData (const QHash<QString, QStringList> &a_networkData)
{
  /* notify model */
  beginResetModel();

  /* vars */
  QVector<NameValueItem> items;
  int current = 0, j = 0;

  /* parse via cycle */
  for (auto i = a_networkData.cbegin(), e = a_networkData.cend(); i != e; i++, j++)
    {
      auto list = i.value();

      if (!list.isEmpty()
          && current == 0)
        current = j;

      items.append (NameValueItem
      {
        i.key(),
        (list.isEmpty()) ? QString() : list.first()
      });
    }

  /* store result */
  try
    {
      auto networks  = _data->module.networks()->as<NetworksModule>();
      networks->setItems (std::move (items));
      networks->setCurrentIndex (current);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();
}

void DapQmlModelOrderList::slotSetTokensListData (const QHash<QString, QString> &a_tokensData)
{
  /* notify model */
  beginResetModel();

  /* vars */
  QVector<NameValueItem> items;

  /* parse via cycle */
  for (auto i = a_tokensData.cbegin(), e = a_tokensData.cend(); i != e; i++)
    items.append (NameValueItem { i.key(), i.value() });

  /* store result */
  try
    {
      auto tokens  = _data->module.tokens()->as<TokensModule>();
      tokens->setItems (std::move (items));
      tokens->setCurrentIndex (0);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();
}

void DapQmlModelOrderList::slotSetOrderAddresses (const QJsonArray &a_list)
{
  /* notify model */
  beginResetModel();

  try
    {
      int index = 0;
      auto &orders = *_data->module.orders()->as<OrdersModule>();
      QHash<QString, QString> nodeIpsMap;

      for (auto i = a_list.constBegin(), e = a_list.constEnd(); i != e; i++, index++)
      {
        auto jobj   = i->toObject();
        auto jitem  = jobj.constBegin();
        nodeIpsMap.insert (jitem.key(), jitem.value().toString());
      }

      orders.installAdressMap (nodeIpsMap);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();
}





/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderListProxyModel::DapQmlModelOrderListProxyModel()
  : QSortFilterProxyModel()
  , m_filter ("")
{
  setSourceModel (DapQmlModelOrderList::instance());
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelOrderListProxyModel::updateCheckedIndex (const QString &a_checkedName)
{
  DapQmlModelOrderList::instance()->setCurrentIndex (
    DapQmlModelOrderList::instance()->indexOf (a_checkedName));
}

void DapQmlModelOrderListProxyModel::setRowFilter (const QString a_filter)
{
  m_filter = a_filter;
  invalidateFilter();
}

/********************************************
 * OVERRIDE
 *******************************************/

bool DapQmlModelOrderListProxyModel::filterAcceptsRow (
  int sourceRow, const QModelIndex &) const
{
  try
    {
      auto orderList  = DapQmlModelOrderList::instance();
      auto module     = orderList->_data->module;
      const QVector<OrderItem> &items = module.orders()->as<OrdersModule>()->items();
      return items.at (sourceRow).location.contains (m_filter, Qt::CaseInsensitive);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return false;
    }
}

/*-----------------------------------------*/
