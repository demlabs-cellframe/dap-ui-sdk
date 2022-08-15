/* INCLUDES */
#include "dapqmlmodelmanageservers.h"

/* DEFS */

/// create servers dummy list
#define ENABLE_SERVERS_DUMMY

/**
 * @brief Server info struct
 */
struct Server
{
  QString name;
  QString address;
  int port;
};

/**
 * @brief Server state struct
 */
struct State
{
  int ping;
  bool favorite;
};

/* VARS */
static QHash<QString, Server> *s_servers  = nullptr; ///< server list
static QHash<QString, State> *s_items     = nullptr; ///< server state list

#ifdef ENABLE_SERVERS_DUMMY
static Server dummyServers[] =
{
  { "AP-1 (South America, USA)",      "18.184.32.170",    443 },
  { "AP-2 (Ireland, Europe)",         "18.184.32.170",    443 },
  { "AP-3 (Seoul, Asia)",             "18.184.32.170",    443 },
  { "AP-4 (Frankfurt, Europe)",       "18.184.32.170",    443 },
  { "AP-5 (South Africa, Sao Paulo)", "18.184.32.170",    443 },
  { "AP-6 (Ireland, Europe)",         "18.184.32.170",    443 },
};
#endif

/* LINKS */
static void appendServer (const Server &a_server);
static void replaceServer (const QString &a_serverName, Server &&a_newServer);
static void removeServer (const QString &a_serverName);
static void updateServerState (const QString &a_serverName, State &&a_item);
static const Server &getServer (const QString &a_serverName);
static const State &getState (const QString &a_serverName);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelManageServers::DapQmlModelManageServers()
{
  s_servers   = new QHash<QString, Server>();
  s_items     = new QHash<QString, State>();
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
