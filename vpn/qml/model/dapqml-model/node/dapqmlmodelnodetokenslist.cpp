/* INCLUDES */

#include "dapqmlmodelnodetokenslist.h"
#include "DapNodeWalletData.h"
#include "dapqmlmodelnodedefines.h"

/* DEFINES */

typedef DapQmlModelNode::FieldId FieldId;

struct DapQmlModelNodeTokensList::Private
{
  int currentIndex = -1;
};

/* VARIABLES */

static DapQmlModelNodeTokensList *s_instance  = nullptr;
static QString s_dummyString;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeTokensList::DapQmlModelNodeTokensList (QObject *a_parent)
  : QAbstractListModel (a_parent)
  , p (new Private)
{

}

DapQmlModelNodeTokensList::~DapQmlModelNodeTokensList()
{
  s_instance  = nullptr;
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeTokensList *DapQmlModelNodeTokensList::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeTokensList;
  return s_instance;
}

QObject *DapQmlModelNodeTokensList::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  return instance();
}

void DapQmlModelNodeTokensList::refresh()
{
  _modelReset();
}

int DapQmlModelNodeTokensList::size() const
{
  return rowCount();
}

int DapQmlModelNodeTokensList::currentIndex() const
{
  return p->currentIndex;
}

bool DapQmlModelNodeTokensList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return false;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
  return true;
}

bool DapQmlModelNodeTokensList::isIndexed() const
{
  return true;
}

bool DapQmlModelNodeTokensList::filterAcceptsRow (int a_row, const QString &a_filter) const
{
  // Если фильтр равен "-" или пустой, показывать все записи
  if (a_filter == "-" || a_filter.isEmpty())
    return true;
  
  // Иначе фильтровать по сети:кошелек
  return data (createIndex (a_row, 0), int (FieldId::misc)).toString() == a_filter;
}

const QString &DapQmlModelNodeTokensList::token() const
{
  const auto &list  = DapNodeWalletData::instance()->tokenBalanceList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex).token;
}

const QString &DapQmlModelNodeTokensList::balance() const
{
  const auto &list  = DapNodeWalletData::instance()->tokenBalanceList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex).balance;
}

const QString &DapQmlModelNodeTokensList::networkAndWallet() const
{
  const auto &list  = DapNodeWalletData::instance()->tokenBalanceList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;

  static QString result;
  const auto &item  = list.at (p->currentIndex);
  result            = item.network + ":" + item.wallet;

  return result;
}

void DapQmlModelNodeTokensList::_modelReset()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeTokensList::rowCount (const QModelIndex &parent) const
{
  const auto &list  = DapNodeWalletData::instance()->tokenBalanceList();
  return list.size();
}

int DapQmlModelNodeTokensList::columnCount (const QModelIndex &parent) const
{
  return DapQmlModelNode::nodeFieldsMap.size();
}

QVariant DapQmlModelNodeTokensList::data (const QModelIndex &index, int role) const
{
  const auto &list  = DapNodeWalletData::instance()->tokenBalanceList();
  if (index.isValid() && index.row() < list.size())
    {
      const auto &item  = list.at (index.row());
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

QHash<int, QByteArray> DapQmlModelNodeTokensList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/*-----------------------------------------*/
