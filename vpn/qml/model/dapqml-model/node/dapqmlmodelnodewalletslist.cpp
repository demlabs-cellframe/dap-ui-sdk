/* INCLUDES */

#include "dapqmlmodelnodewalletslist.h"
#include "DapNodeWalletData.h"
#include "dapqmlmodelnodedefines.h"

/* DEFINES */

typedef DapQmlModelNode::FieldId FieldId;

struct DapQmlModelNodeWalletsList::Private
{
  int currentIndex = -1;
};

/* VARIABLES */

static DapQmlModelNodeWalletsList *s_instance  = nullptr;
static QString s_dummyString;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeWalletsList::DapQmlModelNodeWalletsList (QObject *a_parent)
  : QAbstractListModel (a_parent)
  , p (new Private)
{

}

DapQmlModelNodeWalletsList::~DapQmlModelNodeWalletsList()
{
  s_instance  = nullptr;
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeWalletsList *DapQmlModelNodeWalletsList::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeWalletsList;
  return s_instance;
}

QObject *DapQmlModelNodeWalletsList::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  return instance();
}

void DapQmlModelNodeWalletsList::refresh()
{
  _modelReset();
}

int DapQmlModelNodeWalletsList::size() const
{
  return rowCount();
}

int DapQmlModelNodeWalletsList::currentIndex() const
{
  return p->currentIndex;
}

bool DapQmlModelNodeWalletsList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return false;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
  return true;
}

bool DapQmlModelNodeWalletsList::isIndexed() const
{
  return true;
}

bool DapQmlModelNodeWalletsList::filterAcceptsRow (int a_row, const QString &a_filter) const
{
  if (a_filter.isEmpty() || a_filter == "-")
    return true;

  // Get the wallet data for this row
  const auto &list = DapNodeWalletData::instance()->walletTokenList();
  if (a_row < 0 || a_row >= list.size())
    return false;

  const auto &walletToken = list.at(a_row);
  
  // Check if this wallet belongs to the selected network
  if (walletToken.network != a_filter)
    return false;

  // Get the network fee map to find the native token for this network
  const auto &networkFeeMap = DapNodeWalletData::instance()->networkFeeMap();
  auto it = networkFeeMap.find(walletToken.network);
  
  if (it != networkFeeMap.end()) {
    QString nativeToken = it->feeTicker;
    
    // Only accept wallets that have the native token for this network
    return walletToken.token == nativeToken;
  }
  return false;
}

const QString &DapQmlModelNodeWalletsList::wallet() const
{
  const auto &list  = DapNodeWalletData::instance()->walletTokenList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex).wallet;
}

const QString &DapQmlModelNodeWalletsList::token() const
{
  const auto &list  = DapNodeWalletData::instance()->walletTokenList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex).token;
}

const QString &DapQmlModelNodeWalletsList::network() const
{
  const auto &list  = DapNodeWalletData::instance()->walletTokenList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex).network;
}

void DapQmlModelNodeWalletsList::_modelReset()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeWalletsList::rowCount (const QModelIndex &parent) const
{
  const auto &list  = DapNodeWalletData::instance()->walletTokenList();
  return list.size();
}

int DapQmlModelNodeWalletsList::columnCount (const QModelIndex &parent) const
{
  return DapQmlModelNode::nodeFieldsMap.size();
}

QVariant DapQmlModelNodeWalletsList::data (const QModelIndex &index, int role) const
{
  const auto &list  = DapNodeWalletData::instance()->walletTokenList();
  if (index.isValid() && index.row() < list.size())
    {
      const auto &item  = list.at (index.row());
      switch (FieldId (role))
        {
        case FieldId::name:   return item.wallet;
        case FieldId::value:  return //item.token;
          QString ("%1 %2").arg (item.balance, item.token);
        case FieldId::misc:   return item.network;
        default:
          break;
        }
    }
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelNodeWalletsList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/*-----------------------------------------*/
