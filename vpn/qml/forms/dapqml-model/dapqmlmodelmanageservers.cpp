/* INCLUDES */
#include "dapqmlmodelmanageservers.h"

/* VARS */
static DapQmlModelManageServers *__inst = nullptr;
static QStringList *s_servers           = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelManageServers::DapQmlModelManageServers()
{
  s_servers = new QStringList();
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelManageServers *DapQmlModelManageServers::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelManageServers;
  return __inst;
}

QObject *DapQmlModelManageServers::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED (engine)
  Q_UNUSED (scriptEngine)
  return DapQmlModelManageServers::instance();
}

int DapQmlModelManageServers::length() const
{
  return rowCount();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelManageServers::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_servers->size();
}

int DapQmlModelManageServers::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelManageServers::data(const QModelIndex &index, int role) const
{
  /* check index */
//  if (!index.isValid() || role != 0)
    return QVariant();

//  /* check boundaries */
//  auto list   = DapDataLocal::instance()->getHistorySerialKeyData();
//  if (index.row() >= list.size())
//    return QVariant();

//  /* return value */
//  auto item   = list.at (index.row());
//  return item;
}

QHash<int, QByteArray> DapQmlModelManageServers::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "key");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelManageServers::slotSetup()
{
  beginResetModel();
  *s_servers  = QStringList{
      "keytest1",
      "keytest2",
      "keytest3",
      "keytest4",
      "keytest5",
      "keytest6",
      "keytest7",
      "keytest8",
      "keytest9",
      "keytest10",
      "keytest11",
      "keytest12",
      "keytest13",
      "keytest14",
      "keytest15",
      "keytest16",
      "keytest17",
      "keytest18",
      "keytest19",
      "keytest20",
      "keytest21",
  };
  endResetModel();
}

/*-----------------------------------------*/
