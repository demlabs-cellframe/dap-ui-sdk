/* INCLUDES */
#include "dapqmlmodelorderlist.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/* NAMESPACE */
namespace Dqmol
{

/********************************************
 * DEFS
 *******************************************/

//#define ENABLE_ORDERLIST_SIMULATION
#define DEBUG_MSG qDebug() << "DapQmlModelOrderList" << __func__

typedef DapQmlModelOrderList::Mode Mode;

/*-----------------------------------------*/

struct OrderItem
{
  QString location;
  QString price;
  QString units;
};

struct NameValueItem
{
  QString name;
  QString value;
};

/*-----------------------------------------*/

/**
 * @brief interface class for private model modes
 */

class ModuleInterface
{
  /* VARS */
protected:
  int _currentIndex;

  /* CONSTRUCT/DESTRUCT */
public:
  virtual ~ModuleInterface() {}

  /* VIRTUAL METHODS */
  virtual int size() const = 0;
  virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const = 0;

  /* METHODS */
  int currentIndex() const;
  bool setCurrentIndex (int a_value);

  /// @note ! method can throw std::runtime_error !
  template<class T> T *as()
  {
    return dynamic_cast<T *> (this);
  }
};

/**
 * @brief Name+Value List Module
 *
 * Basic list type.
 * Used for displaying Name + Value items
 */

class NameValueModule : public ModuleInterface
{
  /* VARS */
protected:
  QVector<NameValueItem> _items;

  /* CONSTRUCT/DESTRUCT */
public:
  NameValueModule() {}
  ~NameValueModule() override {};

  /* METHODS */
  const QVector<NameValueItem> &items() const;
  void setItems (const QVector<NameValueItem> &a_items);
  void setItems (QVector<NameValueItem> &&a_items);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

/**
 * @brief Order List Module
 *
 * Complex list.
 * Used to display orders information.
 */

class OrdersModule : public ModuleInterface
{
  /* VARS */
protected:
  QVector<OrderItem> _items;

  /* CONSTRUCT/DESTRUCT */
public:
  OrdersModule() {}
  ~OrdersModule() override {};

  /* METHODS */
  const QVector<OrderItem> &items() const;
  void setItems (const QVector<OrderItem> &a_items);
  void setItems (QVector<OrderItem> &&a_items);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

/**
 * @brief Network List Module
 *
 * Based on basic Name+Value list.
 */

class NetworksModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  NetworksModule() {}
  ~NetworksModule() override {};
};

/**
 * @brief Wallet List Module
 *
 * Based on basic Name+Value list.
 */

class WalletsModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  WalletsModule() {}
  ~WalletsModule() override {};
};

/**
 * @brief Token List Module
 *
 * Based on basic Name+Value list.
 */

class TokensModule : public NameValueModule
{
  /* CONSTRUCT/DESTRUCT */
public:
  TokensModule() {}
  ~TokensModule() override {};
};

/**
 * @brief ModuleInterface pointer wrapper
 *
 * Simply creates module by provided mode
 * and share's it by operator->
 */
class ModuleContainer
{
  Mode _mode;
  QSharedPointer<Dqmol::ModuleInterface> _module;
public:
  ModuleContainer (Mode a_mode = Mode::Invalid)
    : _mode (a_mode)
  {

  }

  Mode mode() const { return _mode; }
  bool setMode (Mode a_value);

  Dqmol::ModuleInterface *operator->()
  {
    if (_module.isNull())
      throw std::runtime_error ("Trying to acces invalid module!");

    return _module.data();
  }
  Dqmol::ModuleInterface &operator&()
  {
    return *operator->();
  }
};

/*-----------------------------------------*/
}; // namespace Dqmol
/*-----------------------------------------*/

using namespace Dqmol;

/*-----------------------------------------*/

enum FieldId
{
  location,
  price,
  priceShort,
  units,

  name,
  value,

  network,
  server,
  wallet,
};

/*-----------------------------------------*/

struct DapQmlModelOrderList::DapQmlModelOrderListData
{
  ModuleContainer module;
  QString serverName;
  QString network;
  QString wallet;
};

/********************************************
 * VARIABLES
 *******************************************/

static QHash<int, QByteArray> s_fields =
{
  { FieldId::location,    "location" },
  { FieldId::price,       "price" },
  { FieldId::priceShort,  "priceShort" },
  { FieldId::units,       "units" },

  { FieldId::name,        "name" },
  { FieldId::value,       "value" },

  { FieldId::network,     "network" },
  { FieldId::server,      "server" },
  { FieldId::wallet,      "wallet" },
};
static QRegularExpression scopesRegExp ("\\(([^)]+)\\)");

static struct
{
  QSharedPointer<ModuleInterface> orders   = QSharedPointer<ModuleInterface> (new OrdersModule);
  QSharedPointer<ModuleInterface> networks = QSharedPointer<ModuleInterface> (new NetworksModule);
  QSharedPointer<ModuleInterface> wallets  = QSharedPointer<ModuleInterface> (new WalletsModule);
  QSharedPointer<ModuleInterface> tokens   = QSharedPointer<ModuleInterface> (new TokensModule);
} s_modules;

/********************************************
 * FUNCTIONS
 *******************************************/

static QString _scopedPrice (const QString &a_value);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelOrderList::DapQmlModelOrderList (QObject *parent)
  : QAbstractListModel {parent}
  , _data (new DapQmlModelOrderListData)
{
#ifdef ENABLE_ORDERLIST_SIMULATION
  auto result = QJsonDocument::fromJson ("[\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024672616E636500\",\n            \"hash\": \"0xF84FC4D96D564E2A54DC6D82A8F90682F0276CB5EF3B26A8A4BF3C18E39EE9F4\",\n            \"node_addr\": \"D860::D9D5::1C57::A6B3\",\n            \"node_location\": \"Europe-France\",\n            \"pkey\": \"0xC6FB9B9370C01F52AD31D1B22A8AB1F1B18AF190EB48BA8F7E24E3DA6E67C8C2\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5202554B00\",\n            \"hash\": \"0x243BBFBBB7C4A360646567A88066F93815E77EB2ECD4B480AF96F7E2B1996E9D\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Europe-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x5206554B00\",\n            \"hash\": \"0x3DB2E19637239001931F2F84F17665164800633E5D263A404ED0264A5CE4F07C\",\n            \"node_addr\": \"E1DB::C873::0B53::ED4B\",\n            \"node_location\": \"Asia-UK\",\n            \"pkey\": \"0x7F336916FE2F638A9457DB00D1841FE8B6234544ABA1785DAA892C421AD05A36\",\n            \"price\": \"0.000000000000000002(2)\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"300\",\n            \"version\": \"3\"\n        },\n        {\n            \"direction\": \"SERV_DIR_SELL\",\n            \"ext\": \"0x52024765726D616E7900\",\n            \"hash\": \"0xF4590296D5844C301FCDA29F7854ADE6FFA2A56044BC1D4995A548342987F09A\",\n            \"node_addr\": \"E02A::FB56::1B0A::19C8\",\n            \"node_location\": \"Europe-Germany\",\n            \"pkey\": \"0xE23EF5A0EA0604EADD74EB2F365809AE222EB88B7EA72C9ADA7AF8B78EA4E21E\",\n            \"price\": \"0.0000000000000012(1200)\",\n            \"price_unit\": \"SECOND\",\n            \"srv_uid\": \"0x0000000000000001\",\n            \"tx_cond_hash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\",\n            \"units\": \"600\",\n            \"version\": \"3\"\n        }\n    ]\n").array();
  slotSetData (result);
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
  if (!_data->module.setMode (a_value))
    return;
  emit sigModeChanged();
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

void DapQmlModelOrderList::setWallet (const QString &a_value)
{
  if (_data->wallet == a_value)
    return;

  _data->wallet = a_value;
  emit sigWalletChanged();
}

static QString _scopedPrice (const QString &a_value)
{
  auto match = scopesRegExp.match (a_value);

  if (match.hasMatch())
    return match.captured (1);

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
  if (!index.isValid())
    return QVariant();

  switch (FieldId (role))
    {
    case FieldId::network:    return _data->network;
    case FieldId::server:     return _data->serverName;
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

  /* parse via cycle */
  for (const auto &item : qAsConst (a_list))
    {
      /* get item */
      QJsonObject joItem = item.toObject();
      if (!joItem.contains ("node_location"))
        continue;

      /* get fields */
      QString loc     = joItem.value ("node_location").toString();
      QString price   = joItem.value ("price").toString();
      QString punit   = joItem.value ("price_unit").toString();

      /* store result */
      items << OrderItem
      {
        std::move (loc),
        std::move (price),
        std::move (punit)
      };
    }

  /* store result */
  try
    {
      s_modules.orders->as<OrdersModule>()->setItems (std::move (items));
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
      const QVector<OrderItem> &items  = s_modules.orders->as<OrdersModule>()->items();
      return items.at (sourceRow).location.contains (m_filter, Qt::CaseInsensitive);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
      return false;
    }
}

/*-----------------------------------------*/

bool ModuleContainer::setMode (Mode a_value)
{
  if (_mode == a_value)
    return false;

  /* change mode */
  _mode   = a_value;

  /* change module */
  switch (_mode)
    {
    case Mode::Orders:    _module = s_modules.orders;   break;
    case Mode::Networks:  _module = s_modules.networks; break;
    case Mode::Wallets:   _module = s_modules.wallets;  break;
    case Mode::Tokens:    _module = s_modules.tokens;   break;
    case Mode::Invalid:
    default:
      _module.reset();
      break;
    }

  return true;
}



/*-----------------------------------------*/
/* NAMESPACE */
namespace Dqmol
{
/*-----------------------------------------*/



/*-----------------------------------------*/
/* ModuleInterface Methods */
/*-----------------------------------------*/

int ModuleInterface::currentIndex() const
{
  return _currentIndex;
}

bool ModuleInterface::setCurrentIndex (int a_value)
{
  if (_currentIndex == a_value)
    return false;

  _currentIndex = a_value;
  return true;
}

/*-----------------------------------------*/
/* NameValueModule Methods */
/*-----------------------------------------*/

const QVector<NameValueItem> &NameValueModule::items() const
{
  return _items;
}

void NameValueModule::setItems (const QVector<NameValueItem> &a_items)
{
  _items  = a_items;
}

void NameValueModule::setItems (QVector<NameValueItem> &&a_items)
{
  _items  = std::move (a_items);
}

int NameValueModule::size() const
{
  return _items.size();
}

QVariant NameValueModule::data (const QModelIndex &index, int role) const
{
  switch (role)
    {
    case FieldId::name:   return _items.at (index.row()).name;
    case FieldId::value:  return _items.at (index.row()).value;
    default:
      break;
    }
  return QVariant();
}

/*-----------------------------------------*/
/* OrdersModule Methods */
/*-----------------------------------------*/

const QVector<OrderItem> &OrdersModule::items() const
{
  return _items;
}

void OrdersModule::setItems (const QVector<OrderItem> &a_items)
{
  _items  = a_items;
}

void OrdersModule::setItems (QVector<OrderItem> &&a_items)
{
  _items  = std::move (a_items);
}

int OrdersModule::size() const
{
  return _items.size();
}

QVariant OrdersModule::data (const QModelIndex &index, int role) const
{
  switch (FieldId (role))
    {
    case FieldId::location:   return _items.at (index.row()).location;
    case FieldId::price:      return _items.at (index.row()).price;
    case FieldId::priceShort: return _scopedPrice (_items.at (index.row()).price);
    case FieldId::units:      return _items.at (index.row()).units;

    default:
      return QVariant();
    }
}

/*-----------------------------------------*/
}; // namespace Dqmol
/*-----------------------------------------*/

/*-----------------------------------------*/
