/* INCLUDES */

#include "dapqmlmodelnodeunitslist.h"
#include "DapNodeWalletData.h"
#include "dapqmlmodelnodedefines.h"

/* DEFINES */

typedef DapQmlModelNode::FieldId FieldId;

struct DapQmlModelNodeUnitsList::Private
{
  QStringList units =
  {
    "All",
    "Second",   // "Secs",
    "Minute",   // "Mins",
    "Hour",     // "Hours"
    "Day",      // "Days",
    "Byte",     // "BYTE",
    "Kilobyte", // "KB",
    "Megabyte", // "MB",
    "Gigabyte", // "GB",
    "Terabyte", // "TB",
  };
  int currentIndex = 0;
};

/* VARIABLES */

static DapQmlModelNodeUnitsList *s_instance  = nullptr;
static QString s_dummyString;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelNodeUnitsList::DapQmlModelNodeUnitsList (QObject *a_parent)
  : QAbstractListModel (a_parent)
  , p (new Private)
{

}

DapQmlModelNodeUnitsList::~DapQmlModelNodeUnitsList()
{
  s_instance  = nullptr;
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelNodeUnitsList *DapQmlModelNodeUnitsList::instance()
{
  if (s_instance == nullptr)
    s_instance = new DapQmlModelNodeUnitsList;
  return s_instance;
}

QObject *DapQmlModelNodeUnitsList::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  return instance();
}

void DapQmlModelNodeUnitsList::refresh()
{
  _modelReset();
}

const QStringList &DapQmlModelNodeUnitsList::units() const
{
  return p->units;
}

void DapQmlModelNodeUnitsList::setUnits (const QStringList &a_units)
{
  beginResetModel();
  p->units  = a_units;
  endResetModel();
}

void DapQmlModelNodeUnitsList::setUnits (QStringList &&a_units)
{
  beginResetModel();
  p->units  = std::move (a_units);
  endResetModel();
}

int DapQmlModelNodeUnitsList::size() const
{
  return rowCount();
}

int DapQmlModelNodeUnitsList::currentIndex() const
{
  return p->currentIndex;
}

bool DapQmlModelNodeUnitsList::setCurrentIndex (int a_index)
{
  if (p->currentIndex == a_index)
    return false;

  p->currentIndex = a_index;
  emit sigCurrentIndexChanged();
  return true;
}

bool DapQmlModelNodeUnitsList::isIndexed() const
{
  return true;
}

bool DapQmlModelNodeUnitsList::filterAcceptsRow (int a_row, const QString &a_filter) const
{
  return data (createIndex (a_row, 0), int (FieldId::misc)).toString() == a_filter;
}

const QString &DapQmlModelNodeUnitsList::unit() const
{
  if (p->currentIndex < 0 || p->currentIndex >= p->units.size())
    return s_dummyString;
  return p->units.at (p->currentIndex);
}

void DapQmlModelNodeUnitsList::_modelReset()
{
  beginResetModel();
  endResetModel();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelNodeUnitsList::rowCount (const QModelIndex &parent) const
{
  return p->units.size();
}

int DapQmlModelNodeUnitsList::columnCount (const QModelIndex &parent) const
{
  return DapQmlModelNode::nodeFieldsMap.size();
}

QVariant DapQmlModelNodeUnitsList::data (const QModelIndex &index, int role) const
{
  if (index.isValid() && index.row() < p->units.size())
    if (FieldId (role) == FieldId::name)
      return p->units.at (index.row());
  return QVariant();
}

QHash<int, QByteArray> DapQmlModelNodeUnitsList::roleNames() const
{
  return DapQmlModelNode::nodeFieldsMap;
}

/*-----------------------------------------*/
