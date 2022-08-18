/* INCLUDES */
#include "dapqmlmodelmanageservers.h"
#include "dapqml-abstract/abstractservermanager.h"
#include <QImage>
#include <QDebug>
#include <array>

/* DEFS */

/// create servers dummy list
#define ENABLE_SERVERS_DUMMY

/* VARS */
static QSharedPointer<AbstractServerManager> s_manager;

#ifdef ENABLE_SERVERS_DUMMY
static std::array<AbstractServerManager::Server, 6> dummyServers =
//static AbstractServerManager::Server dummyServers[] =
{
  AbstractServerManager::Server{ "AP-1 (South America, USA)",      "18.184.32.170",    443 },
  { "AP-2 (Ireland, Europe)",         "18.184.32.170",    443 },
  { "AP-3 (Seoul, Asia)",             "18.184.32.170",    443 },
  { "AP-4 (Frankfurt, Europe)",       "18.184.32.170",    443 },
  { "AP-5 (South Africa, Sao Paulo)", "18.184.32.170",    443 },
  { "AP-6 (Ireland, Europe)",         "18.184.32.170",    443 },
};
#endif

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelManageServers::DapQmlModelManageServers()
{

}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelManageServers *DapQmlModelManageServers::instance()
{
  static DapQmlModelManageServers *__inst = nullptr;
  if (__inst == nullptr)
    __inst  = new DapQmlModelManageServers;
  return __inst;
}

QObject *DapQmlModelManageServers::singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED (engine)
  Q_UNUSED (scriptEngine)
  return DapQmlModelManageServers::instance();
}

int DapQmlModelManageServers::length() const
{
  return rowCount();
}

void DapQmlModelManageServers::installManager (QSharedPointer<AbstractServerManager> a_manager)
{
  s_manager = a_manager;
}

void DapQmlModelManageServers::fillDummyList()
{
#ifdef ENABLE_SERVERS_DUMMY
  if (s_manager.isNull())
    return;

  for (auto &it : dummyServers)
    s_manager->append (std::move (it));
#endif // ENABLE_SERVERS_DUMMY
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelManageServers::rowCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  if (s_manager.isNull())
    return 0;

  return s_manager->size();
}

int DapQmlModelManageServers::columnCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelManageServers::data (const QModelIndex &index, int role) const
{
  /* check index */
  if (!index.isValid())
    return QVariant();

  /* check boundaries */
  auto list   = s_manager->keys();
  if (index.row() >= list.size())
    return QVariant();

  /* return value */
  switch (role)
    {

    case 0: // icon
      return "ic_conn-4";

    case 1: // name
      return list.at (index.row());

    case 2: // favorite
      return index.row() == 1; // dummy check

    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelManageServers::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "icon");
  names.insert (1, "name");
  names.insert (2, "favorite");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelManageServers::slotSetup()
{
  beginResetModel();
//  *s_servers  = QStringList{
//      "keytest1",
//      "keytest2",
//      "keytest3",
//      "keytest4",
//      "keytest5",
//      "keytest6",
//      "keytest7",
//      "keytest8",
//      "keytest9",
//      "keytest10",
//      "keytest11",
//      "keytest12",
//      "keytest13",
//      "keytest14",
//      "keytest15",
//      "keytest16",
//      "keytest17",
//      "keytest18",
//      "keytest19",
//      "keytest20",
//      "keytest21",
//  };
  endResetModel();
}

/*-----------------------------------------*/
