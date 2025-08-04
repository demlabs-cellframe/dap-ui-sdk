/* INCLUDES */

#include "dapqmlmodelnodenetworkslist.h"
#include "DapNodeWalletData.h"
#include "dapqmlmodelnodedefines.h"
#include "DapDeveloperModeManager.h"

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

bool DapQmlModelNodeNetworksList::filterAcceptsRow (int a_row, const QString &a_filter) const
{
  return true;
}

const QString &DapQmlModelNodeNetworksList::network() const
{
  const auto &list  = DapNodeWalletData::instance()->networkList();
  if (p->currentIndex < 0 || p->currentIndex >= list.size())
    return s_dummyString;
  return list.at (p->currentIndex);
}

QString DapQmlModelNodeNetworksList::selectDefaultNetwork()
{
  const auto &list = DapNodeWalletData::instance()->networkList();
  if (list.isEmpty()) {
    return QString();
  }

  // Check if we're in developer mode
  bool isDeveloperMode = DapDeveloperModeManager::instance()->isDeveloperModeEnabled();
  
  if (isDeveloperMode) {
    // In developer mode, don't auto-select - let user choose
    return QString();
  }

  // In user mode, automatically select network based on brand
  QString defaultNetwork = DapDeveloperModeManager::instance()->getNetworkForCurrentBrand();
  
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) == defaultNetwork) {
      setCurrentIndex(i);
      return defaultNetwork;
    }
  }

  // If brand network not found, select first available network
  if (!list.isEmpty()) {
    QString firstNetwork = list.at(0);
    setCurrentIndex(0);
    return firstNetwork;
  }

  return QString();
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
  if (index.isValid() && index.row() < list.size())
    if (FieldId (role) == FieldId::name)
      return list.at (index.row());
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelNodeNetworksList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/*-----------------------------------------*/
