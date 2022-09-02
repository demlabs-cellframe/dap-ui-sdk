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
  AbstractServerManager::Server{ "AP-1 (South America, USA)",      "18.184.32.170",    443, false },
  { "AP-2 (Ireland, Europe)",         "18.184.32.170",    443, true },
  { "AP-3 (Seoul, Asia)",             "18.184.32.171",    443, false },
  { "AP-4 (Frankfurt, Europe)",       "18.184.32.172",    443, true },
  { "AP-5 (South Africa, Sao Paulo)", "18.184.32.173",    443, true },
  { "AP-6 (Ireland, Europe)",         "18.184.32.174",    443, false },
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

#define _parseField(map,item,name,conversion) \
  if (map.contains (#name)) \
    item.name     = map[#name].conversion();

static void parseServerData (const QVariant &a_data, /* out */ AbstractServerManager::Server &a_server)
{
  auto value        = a_data.toMap();

  _parseField (value, a_server, name, toString);
  _parseField (value, a_server, address, toString);
  _parseField (value, a_server, port, toInt);
  _parseField (value, a_server, favorite, toBool);

//  if (value.contains ("name"))
//    a_server.name     = value["name"].toString();

//  if (value.contains ("address"))
//    a_server.address  = value["address"].toString();

//  if (value.contains ("port"))
//    a_server.port     = value["port"].toInt();

//  if (value.contains ("favorite"))
//    a_server.favorite = value["favorite"].toBool();
}

void DapQmlModelManageServers::add (const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* get new server info */
  AbstractServerManager::Server newServer;
  parseServerData (a_data, newServer);

  /* store result */
  s_manager->append (std::move (newServer));
}

void DapQmlModelManageServers::edit (int a_index, const QVariant &a_data)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* check boundaries */
  if (a_index >= s_manager->size())
    return;

  /* get source item */
  AbstractServerManager::Server item = s_manager->server (a_index);

  /* modify data */
  parseServerData (a_data, item);

  /* store result */
  s_manager->setServer (a_index, std::move (item));
}

void DapQmlModelManageServers::remove (int a_index)
{
  /* check if manager installed */
  if (s_manager.isNull())
    return;

  /* check boundaries */
  if (a_index >= s_manager->size())
    return;

  /* perform removing */
  s_manager->remove (a_index);
}

void DapQmlModelManageServers::refreshContent()
{
  beginResetModel();
  endResetModel();
}

QVariant DapQmlModelManageServers::value (int a_index, const QString &a_name)
{
  return data (index (a_index, 0), roleNames().key (a_name.toUtf8()));
}

void DapQmlModelManageServers::fillDummyList()
{
#ifdef ENABLE_SERVERS_DUMMY
  /* check if manager installed */
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

  /* check if manager installed */
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
  /* check if manager installed */
  if (s_manager.isNull())
    return QVariant();

  /* check index */
  if (!index.isValid())
    return QVariant();

  /* check boundaries */
  if (index.row() >= s_manager->size())
    return QVariant();

  /* return value */
  switch (role)
    {

    case 0: // icon
      return QString ("ic_conn-%1").arg (index.row() % 6);//"ic_conn-4";

    case 1: // name
      {
        const auto &server = s_manager->server (index.row());
        return server.name;
      }
     // return list.at (index.row());

    case 2: // favorite
    {
      const auto &server = s_manager->server (index.row());
      return server.favorite;
    }

    case 3: // address
    {
      const auto &server = s_manager->server (index.row());
      return server.address;
    }

    case 4: // port
    {
      const auto &server = s_manager->server (index.row());
      return server.port;
    }

    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelManageServers::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "icon");
  names.insert (1, "name");
  names.insert (2, "favorite");
  names.insert (3, "address");
  names.insert (4, "port");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelManageServers::slotSetup()
{

}

/*-----------------------------------------*/
