/* INCLUDES */
#include "dapqmlmodelorderlist.h"

#include <QTimer>
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
//#define ENABLE_NAME_VALUE_DUMMY
#define DEBUG_MSG qDebug() << "DapQmlModelOrderList" << __func__

typedef DapQmlModelOrderList::Mode Mode;

/*-----------------------------------------*/

struct OrderItem
{
  QString location;
  QString price;
  QString units;
  QString server;
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
  ModuleInterface() : _currentIndex (-1) {}
  virtual ~ModuleInterface() {}

  /* VIRTUAL METHODS */
  virtual int size() const = 0;
  virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
  virtual bool setCurrentIndex (int a_value);

  virtual const QString &name() const = 0;
  virtual const QString &value() const = 0;

  /* METHODS */
  int currentIndex() const;

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
  const QString &name() const override;
  const QString &value() const override;
  //bool setCurrentIndex (int a_value) override;
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

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
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

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
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

  /* OVERRIDE */
  const QString &name() const override;
  const QString &value() const override;
  bool setCurrentIndex (int a_value) override;
};

/**
 * @brief Unit List Module
 *
 * Basic unit select list.
 */

class UnitsModule : public ModuleInterface
{
  /* VARS */
protected:
  QStringList _items;

  /* CONSTRUCT/DESTRUCT */
public:
  UnitsModule();
  ~UnitsModule() override {};

  /* METHODS */
  const QStringList &items() const;
  void setItems (const QStringList &a_items);
  void setItems (QStringList &&a_items);

  /* OVERRIDE */
  int size() const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setCurrentIndex (int a_value) override;
  const QString &name() const override;
  const QString &value() const override;
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
  /* order flields */
  location,
  price,
  priceShort,
  units,
  server,

  /* name + value fields */
  name,
  value,

  /* model fields */
  network,
  wallet,
};

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
  QSharedPointer<ModuleInterface> units    = QSharedPointer<ModuleInterface> (new UnitsModule);
} s_modules;

static const QString s_dummyString; // string that is returned as reference when certain conditions met

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
    _data->network  = s_modules.networks->name();
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
    _data->wallet  = s_modules.wallets->name();
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
    _data->token  = s_modules.tokens->name();
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
    _data->unit  = s_modules.units->name();
  else
    {
      if (_data->unit == a_value)
        return;
      else
        _data->unit = a_value;
    }

  emit sigUnitChanged();
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
      QString server  = joItem.value ("node_location").toString();

      /* store result */
      items << OrderItem
      {
        std::move (loc),
        std::move (price),
        std::move (punit),
        std::move (server)
      };
    }

  /* store result */
  try
    {
      auto orders = s_modules.orders->as<OrdersModule>();
      orders->setItems (std::move (items));
      orders->setCurrentIndex (0);
    }
  catch (const std::exception &e)
    {
      DEBUG_MSG << "Exception occurred:" << e.what();
    }

  /* notify model */
  endResetModel();
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
      auto wallets  = s_modules.wallets->as<WalletsModule>();
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
      auto networks  = s_modules.networks->as<NetworksModule>();
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
      auto tokens  = s_modules.tokens->as<TokensModule>();
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
    case Mode::Units:     _module = s_modules.units;    break;
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
    case FieldId::server:     return _items.at (index.row()).server;

    default:
      return QVariant();
    }
}

const QString &OrdersModule::name() const
{
  return s_dummyString;
}

const QString &OrdersModule::value() const
{
  return s_dummyString;
}

/*-----------------------------------------*/
/* NetworksModule Methods */
/*-----------------------------------------*/

const QString &NetworksModule::name() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).name;
}

const QString &NetworksModule::value() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).value;
}

bool NetworksModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    DapQmlModelOrderList::instance()->setNetwork();

  return result;
}

/*-----------------------------------------*/
/* WalletsModule Methods */
/*-----------------------------------------*/

const QString &WalletsModule::name() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).name;
}

const QString &WalletsModule::value() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).value;
}

bool WalletsModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    DapQmlModelOrderList::instance()->setWallet();

  return result;
}

/*-----------------------------------------*/
/* TokensModule Methods */
/*-----------------------------------------*/

const QString &TokensModule::name() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).name;
}

const QString &TokensModule::value() const
{
  if (_currentIndex < 0)
    return s_dummyString;
  return _items.at (_currentIndex).value;
}

bool TokensModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    DapQmlModelOrderList::instance()->setToken();

  return result;
}

/*-----------------------------------------*/
/* UnitsModule Methods */
/*-----------------------------------------*/

UnitsModule::UnitsModule()
{
  _items =
  QStringList
  {
    "days",
    "MB"
  };
  _currentIndex = 0;
  DapQmlModelOrderList::instance()->setUnit (_items.first());
}

const QStringList &UnitsModule::items() const
{
  return _items;
}

void UnitsModule::setItems (const QStringList &a_items)
{
  _items  = a_items;
}

void UnitsModule::setItems (QStringList &&a_items)
{
  _items  = std::move (a_items);
}

int UnitsModule::size() const
{
  return _items.size();
}

QVariant UnitsModule::data (const QModelIndex &index, int role) const
{
  switch (role)
    {
    case FieldId::name:   return _items.at (index.row());
    default:
      return QVariant();
    }
}

bool UnitsModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    DapQmlModelOrderList::instance()->setUnit();

  return result;
}

const QString &UnitsModule::name() const
{
  return _items.at (_currentIndex);
}

const QString &UnitsModule::value() const
{
  return s_dummyString; //_items.at (_currentIndex);
}

/*-----------------------------------------*/
}; // namespace Dqmol
/*-----------------------------------------*/

/*-----------------------------------------*/
