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

int DapQmlModelNodeNetworksList::currentIndex() const
{
  return p->currentIndex;
}

void DapQmlModelNodeNetworksList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
}

const QString &DapQmlModelNodeNetworksList::name() const
{
  const auto &list  = DapNodeWalletData::instance()->networkList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex);
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
