/* INCLUDES */
#include "orderlistmodules.h"
#include "DapNodeWalletData.h"

#include <QTimer>

/* NAMESPACE */
namespace OrderListModule
{

/* DEFS */
typedef DapQmlModelOrderList::FieldId FieldId;

/* VARS */
static QRegularExpression scopesRegExp ("\\(([^)]+)\\)");
static const QString s_dummyString; // string that is returned as reference when certain conditions met

/* FUNCTIONS */
QString _scopedPrice (const QString &a_value);

/*-----------------------------------------*/
/* ModuleInterface */
/*-----------------------------------------*/

ModuleInterface::ModuleInterface()
  : _currentIndex (-1)
{

}

ModuleInterface::~ModuleInterface()
{

}

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

//const QVector<NameValueItem> &NameValueModule::items() const
//{
//  return _items;
//}

//void NameValueModule::setItems (const QVector<NameValueItem> &a_items)
//{
//  _items  = a_items;
//}

//void NameValueModule::setItems (QVector<NameValueItem> &&a_items)
//{
//  _items  = std::move (a_items);
//}

//int NameValueModule::size() const
//{
//  return _items.size();
//}

//QVariant NameValueModule::data (const QModelIndex &index, int role) const
//{
//  if (index.row() < _items.size())
//    {
//      switch (FieldId (role))
//        {
//        case FieldId::name:   return _items.at (index.row()).name;
//        case FieldId::value:  return _items.at (index.row()).value;
//        default:
//          break;
//        }
//    }
//  return QVariant();
//}

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

OrderItem &OrdersModule::operator[] (int a_index)
{
  return _items[a_index];
}

int OrdersModule::size() const
{
  return _items.size();
}

QVariant OrdersModule::data (const QModelIndex &index, int role) const
{
  if (index.row() < _items.size())
    {
      switch (FieldId (role))
        {
        case FieldId::location:    return _items.at (index.row()).location;
        case FieldId::node_addr:   return _items.at (index.row()).node_addr;
        case FieldId::price:       return _items.at (index.row()).price;
        case FieldId::priceShort:  return _scopedPrice (_items.at (index.row()).price);
        case FieldId::units:       return _items.at (index.row()).units;
        case FieldId::units_value: return _items.at (index.row()).units_value;
        case FieldId::server:      return _items.at (index.row()).server;
        case FieldId::hash:        return _items.at (index.row()).hash;
        case FieldId::ipAddress:   return _items.at (index.row()).ipAddress;

        default:
          break;
        }
    }
  return QVariant();
}

const QString &OrdersModule::name() const
{
  if (_currentIndex < 0 || _currentIndex >= _items.size())
    return s_dummyString;
  return _items.at (_currentIndex).hash;
}

const QString &OrdersModule::value() const
{
  return s_dummyString;
}

const QString &OrdersModule::misc() const
{
  return s_dummyString;
}

bool OrdersModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    emit DapQmlModelOrderList::instance()->sigOrderSelected (name());

  return result;
}

void OrdersModule::installAdressMap (const QHash<QString, QString> &a_map)
{
  for (auto i = _items.begin(), e = _items.end(); i != e; i++)
    i->ipAddress  = a_map.value (i->node_addr, QString());
}

/*-----------------------------------------*/
/* NetworksModule Methods */
/*-----------------------------------------*/

int NetworksModule::size() const
{
  return DapNodeWalletData::instance()->networkList().size();
}

QVariant NetworksModule::data (const QModelIndex &index, int role) const
{
  const auto &data  = DapNodeWalletData::instance()->networkList();
  if (index.row() < data.size())
    {
      switch (FieldId (role))
        {
        case FieldId::name:   return data.at (index.row());
        default:
          break;
        }
    }
  return QVariant();
}

const QString &NetworksModule::name() const
{
  const auto &data  = DapNodeWalletData::instance()->networkList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex);
}

const QString &NetworksModule::value() const
{
  return s_dummyString;
}

const QString &NetworksModule::misc() const
{
  return s_dummyString;
}

bool NetworksModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    {
      auto orderList  = DapQmlModelOrderList::instance();
      orderList->setNetwork();
      // emit orderList->sigNetworkUpdated (name());
    }

  return result;
}

/*-----------------------------------------*/
/* WalletsModule Methods */
/*-----------------------------------------*/

int WalletsModule::size() const
{
  return DapNodeWalletData::instance()->walletTokenList().size();
}

QVariant WalletsModule::data (const QModelIndex &index, int role) const
{
  const auto &data  = DapNodeWalletData::instance()->walletTokenList();
  if (index.row() < data.size())
    {
      const auto &item  = data.at (index.row());
      switch (FieldId (role))
        {
        case FieldId::name:   return item.wallet;
        case FieldId::value:  return item.token;
        case FieldId::misc:   return item.network;
        default:
          break;
        }
    }
  return QVariant();
}

const QString &WalletsModule::name() const
{
  const auto &data  = DapNodeWalletData::instance()->walletTokenList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex).wallet;
}

const QString &WalletsModule::value() const
{
  const auto &data  = DapNodeWalletData::instance()->walletTokenList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex).token;
}

const QString &WalletsModule::misc() const
{
  const auto &data  = DapNodeWalletData::instance()->walletTokenList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex).network;
}

bool WalletsModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    {
      auto orderList  = DapQmlModelOrderList::instance();
      orderList->setWallet();
      // emit orderList->sigWalletUpdated (name());
    }

  return result;
}

/*-----------------------------------------*/
/* TokensModule Methods */
/*-----------------------------------------*/

int TokensModule::size() const
{
  return DapNodeWalletData::instance()->tokenBalanceList().size();
}

QVariant TokensModule::data (const QModelIndex &index, int role) const
{
  const auto &data  = DapNodeWalletData::instance()->tokenBalanceList();
  if (index.row() < data.size())
    {
      const auto &item  = data.at (index.row());
      switch (FieldId (role))
        {
        case FieldId::name:   return item.token;
        case FieldId::value:  return item.balance;
        case FieldId::misc:   return item.network + ":" + item.wallet;
        default:
          break;
        }
    }
  return QVariant();
}

const QString &TokensModule::name() const
{
  const auto &data  = DapNodeWalletData::instance()->tokenBalanceList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex).token;
}

const QString &TokensModule::value() const
{
  const auto &data  = DapNodeWalletData::instance()->tokenBalanceList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;
  return data.at (_currentIndex).balance;
}

const QString &TokensModule::misc() const
{
  const auto &data  = DapNodeWalletData::instance()->tokenBalanceList();
  if (_currentIndex < 0 || _currentIndex >= data.size())
    return s_dummyString;

  static QString result;
  const auto &item  = data.at (_currentIndex);
  result            = item.network + ":" + item.wallet;

  return result;
}

bool TokensModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    {
      auto orderList  = DapQmlModelOrderList::instance();
      orderList->setToken();
      // emit orderList->sigTokenUpdated (name());
    }

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
    "All",
    "Second",   // "Secs",
    "Minute",   // "Mins",
    "Hour",     // "Hours"
    "Day",      // "Days",
    "Kilobyte", // "KB",
    "Megabyte", // "MB",
    "Gigabyte", // "GB",
    "Terabyte", // "TB",
  };
  _currentIndex = 0;
  QTimer::singleShot (100, [this] {DapQmlModelOrderList::instance()->setUnit (_items.at (_currentIndex));});
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
  if (index.row() < _items.size())
    {
      switch (FieldId (role))
        {
        case FieldId::name:   return _items.at (index.row());
        default: break;
        }
    }
  return QVariant();
}

bool UnitsModule::setCurrentIndex (int a_value)
{
  bool result = ModuleInterface::setCurrentIndex (a_value);

  if (result)
    {
      auto orderList  = DapQmlModelOrderList::instance();
      orderList->setUnit();
      // emit orderList->sigUnitUpdated (name());
    }

  return result;
}

const QString &UnitsModule::name() const
{
  if (_currentIndex < 0 || _currentIndex >= _items.size())
    return s_dummyString;
  return _items.at (_currentIndex);
}

const QString &UnitsModule::value() const
{
  return s_dummyString; //_items.at (_currentIndex);
}

const QString &UnitsModule::misc() const
{
  return s_dummyString;
}

/*-----------------------------------------*/
/* ModuleContainer */
/*-----------------------------------------*/

ModuleContainer::ModuleContainer (DapQmlModelOrderList::Mode a_mode)
  : _mode (a_mode)
{
  //_orders   = ModuleInterfacePtr (new OrdersModule);
  _networks = ModuleInterfacePtr (new NetworksModule);
  _wallets  = ModuleInterfacePtr (new WalletsModule);
  _tokens   = ModuleInterfacePtr (new TokensModule);
  _units    = ModuleInterfacePtr (new UnitsModule);
}

DapQmlModelOrderList::Mode ModuleContainer::mode() const
{
  return _mode;
}

bool ModuleContainer::setMode (DapQmlModelOrderList::Mode a_value)
{
  if (_mode == a_value)
    return false;

//  typedef DapQmlModelOrderList::Mode Mode;

  /* change mode */
  _mode   = a_value;

//  /* change module */
//  switch (_mode)
//    {
//    //case Mode::Orders:    _module = _orders;   break;
//    case Mode::Networks:  _module = _networks; break;
//    case Mode::Wallets:   _module = _wallets;  break;
//    case Mode::Tokens:    _module = _tokens;   break;
//    case Mode::Units:     _module = _units;    break;
//    case Mode::Invalid:
//    default:
//      _module.reset();
//      break;
//    }
  _module = _units;

  return true;
}

ModuleInterface *ModuleContainer::operator->()
{
  if (_module.isNull())
    throw std::runtime_error ("Trying to acces invalid module!");

  return _module.data();
}

ModuleInterface &ModuleContainer::operator&()
{
  return *operator->();
}

/*-----------------------------------------*/

QString _scopedPrice (const QString &a_value)
{
  auto match = scopesRegExp.match (a_value);

  if (match.hasMatch())
    return match.captured (1);

  return QString();
}

/*-----------------------------------------*/
}
/*-----------------------------------------*/
