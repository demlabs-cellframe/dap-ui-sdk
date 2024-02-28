/* INCLUDES */

#include "dapqmlmodelnodenetworkslist.h"
#include "DapNodeWalletData.h"
#include "dapqmlmodelnodedefines.h"

/* DEFINES */

typedef DapQmlModelNode::FieldId FieldId;

struct DapQmlModelNodeNetworksList::Private
{
  int currentIndex = -1;
};

/* VARIABLES */

static DapQmlModelNodeNetworksList *s_instance  = nullptr;
static QString s_dummyString;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeNetworksList::DapQmlModelNodeNetworksList (QObject *a_parent)
  : QAbstractListModel (a_parent)
  , p (new Private)
{

}

DapQmlModelNodeNetworksList::~DapQmlModelNodeNetworksList()
{
  s_instance  = nullptr;
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeNetworksList *DapQmlModelNodeNetworksList::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeNetworksList;
  return s_instance;
}

QObject *DapQmlModelNodeNetworksList::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  return instance();
}

void DapQmlModelNodeNetworksList::refresh()
{
  _modelReset();
}

int DapQmlModelNodeNetworksList::size() const
{
  return rowCount();
}

int DapQmlModelNodeNetworksList::currentIndex() const
{
  return p->currentIndex;
}

bool DapQmlModelNodeNetworksList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return false;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
  return true;
}

bool DapQmlModelNodeNetworksList::isIndexed() const
{
  return false;
}

const QString &DapQmlModelNodeNetworksList::network() const
{
  const auto &list  = DapNodeWalletData::instance()->networkList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex);
}

void DapQmlModelNodeNetworksList::_modelReset()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeNetworksList::rowCount (const QModelIndex &parent) const
{
  const auto &list  = DapNodeWalletData::instance()->networkList();
  return list.size();
}

int DapQmlModelNodeNetworksList::columnCount (const QModelIndex &parent) const
{
  return DapQmlModelNode::nodeFieldsMap.size();
}

QVariant DapQmlModelNodeNetworksList::data (const QModelIndex &index, int role) const
{
  const auto &list  = DapNodeWalletData::instance()->networkList();
  if (index.row() < list.size())
    if (FieldId (role) == FieldId::name)
      return list.at (index.row());
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelNodeNetworksList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/*-----------------------------------------*/
