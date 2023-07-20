/* INCLUDES */
#include "dapqmlmodelorderlist.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/* DEFS */
#define ENABLE_ORDERLIST_SIMULATION

enum FieldId
{
  location,
  price,
  priceShort,
  units,

  network,
  server,
  wallet,
};

struct DapQmlModelOrderListItem
{
  QString location;
  QString price;
  QString units;
};

struct DapQmlModelOrderList::DapQmlModelOrderListData
{
  QList<DapQmlModelOrderListItem> items;
  QString serverName;
  QString network;
  QString wallet;
  int currentIndex;
};

/* VARS */
static QHash<int, QByteArray> s_fields =
{
  { FieldId::location,    "location" },
  { FieldId::price,       "price" },
  { FieldId::priceShort,  "priceShort" },
  { FieldId::units,       "units" },
  { FieldId::network,     "network" },
  { FieldId::server,      "server" },
  { FieldId::wallet,      "wallet" },
};
static QRegularExpression scopesRegExp ("\\(([^)]+)\\)");

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderList::DapQmlModelOrderList (QObject *parent)
  : QAbstractListModel {parent}
  , _data (new DapQmlModelOrderListData)
{
#ifdef ENABLE_ORDERLIST_SIMULATION
  auto result = QJsonDocument::fromJson ("[\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024672616E636500\",\n            \"hash\": \"0xF84FC4D96D564E2A54DC6D82A8F90682F0276CB5EF3B26A8A4BF3C18E39EE9F4\",\n            \"node_addr\": \"D860::D9D5::1C57::A6B3\",\n            \"node_location\": \"Europe-France\",\n            \"pkey\": \"0xC6FB9B9370C01F52AD31D1B22A8AB1F1B18AF190EB48BA8F7E24E3DA6E67C8C2\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5202554B00\",\n            \"hash\": \"0x243BBFBBB7C4A360646567A88066F93815E77EB2ECD4B480AF96F7E2B1996E9D\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Europe-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5206554B00\",\n            \"hash\": \"0x3DB2E19637239001931F2F84F17665164800633E5D263A404ED0264A5CE4F07C\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Asia-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.000000000000000002(2)\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024765726D616E7900\",\n            \"hash\": \"0xF4590296D5844C301FCDA29F7854ADE6FFA2A56044BC1D4995A548342987F09A\",\n            \"node_addr\": \"E02A::FB56::1B0A::19C8\",\n            \"node_location\": \"Europe-Germany\",\n            \"pkey\": \"0xE23EF5A0EA0604EADD74EB2F365809AE222EB88B7EA72C9ADA7AF8B78EA4E21E\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"600\",\n            \"version\": \"3\"\n        }\n    ]\n").array();
  slotSetData (QVariantMap {{"data", result}});
  setServerName ("Server name");
  setNetwork ("TestNetwork");
  setWallet ("TestWallet");
#endif // ENABLE_ORDERLIST_SIMULATION
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
  static DapQmlModelOrderList i;
  return &i;
}

QObject *DapQmlModelOrderList::singletonProvider (QQmlEngine *, QJSEngine *)
{
  return instance();
}

int DapQmlModelOrderList::length() const
{
  return _data->items.size();
}

int DapQmlModelOrderList::currentIndex() const
{
  return _data->currentIndex;
}

void DapQmlModelOrderList::setCurrentIndex (int a_index)
{
  if (_data->currentIndex == a_index)
    return;

  _data->currentIndex = a_index;
  emit sigCurrentIndexChanged();
}

int DapQmlModelOrderList::indexOf (const QString &a_location)
{
  int index = 0;
  for (auto i = _data->items.cbegin(), e = _data->items.cend(); i != e; i++, index++)
    if (i->location == a_location)
      return index;
  return -1;
}

QString DapQmlModelOrderList::serverName() const
{
  return _data->serverName;
}

void DapQmlModelOrderList::setServerName (const QString &a_value)
{
  if (_data->serverName == a_value)
    return;

  _data->serverName = a_value;
  emit sigServerNameChanged();
}

QString DapQmlModelOrderList::network() const
{
  return _data->network;
}

void DapQmlModelOrderList::setNetwork (const QString &a_value)
{
  if (_data->network == a_value)
    return;

  _data->network = a_value;
  emit sigNetworkChanged();
}

QString DapQmlModelOrderList::wallet() const
{
  return _data->wallet;
}

void DapQmlModelOrderList::setWallet(const QString &a_value)
{
  if (_data->wallet == a_value)
    return;

  _data->wallet = a_value;
  emit sigWalletChanged();
}

QString DapQmlModelOrderList::_scopedPrice (const QString &a_value)
{
  auto match = scopesRegExp.match (a_value);

  if (match.hasMatch())
    return match.captured(1);

  return QString();
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
  if (!index.isValid()
      || index.row() < 0
      || index.row() >= length())
    return QVariant();

  switch (FieldId (role))
  {
  case FieldId::location:   return _data->items.at (index.row()).location;
  case FieldId::price:      return _data->items.at (index.row()).price;
  case FieldId::priceShort: return _scopedPrice (_data->items.at (index.row()).price);
  case FieldId::units:      return _data->items.at (index.row()).units;

  case FieldId::network:    return _data->network;
  case FieldId::server:     return _data->serverName;
  case FieldId::wallet:     return _data->wallet;
  }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelOrderList::roleNames() const
{
  return s_fields;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelOrderList::slotSetData (const QMap<QString, QVariant> &a_orderListData)
{
  if (a_orderListData.isEmpty())
    return;

  _data->items.clear();

  QJsonArray array  = a_orderListData.value ("data").toJsonArray();

  for (const auto& item : qAsConst (array))
  {
    QJsonObject joItem = item.toObject();
    if (!joItem.contains ("node_location"))
      continue;

    QString loc     = joItem.value ("node_location").toString();
    QString price   = joItem.value ("price").toString();
    QString punit   = joItem.value ("price_unit").toString();

    _data->items << DapQmlModelOrderListItem{
      std::move (loc),
      std::move (price),
      std::move (punit)
    };
  }
}





/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderListProxyModel::DapQmlModelOrderListProxyModel()
  : QSortFilterProxyModel()
  , m_filter("")
{
  setSourceModel (DapQmlModelOrderList::instance());
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelOrderListProxyModel::updateCheckedIndex (const QString &a_checkedName)
{
  DapQmlModelOrderList::instance()->setCurrentIndex(
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

bool DapQmlModelOrderListProxyModel::filterAcceptsRow(
  int sourceRow, const QModelIndex &) const
{
  const QList<DapQmlModelOrderListItem> &items  = DapQmlModelOrderList::instance()->_data->items;
  return items.at (sourceRow).location.contains (m_filter, Qt::CaseInsensitive);
}

/*-----------------------------------------*/
