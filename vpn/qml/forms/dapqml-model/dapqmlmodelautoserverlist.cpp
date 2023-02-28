/* INCLUDES */
#include "dapqmlmodelautoserverlist.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelAutoServerList::DapQmlModelAutoServerList ()
  : _abstractServerList (DapServerList::instance())
{
  connect (_serverList, &QAbstractItemModel::modelReset, this, [this] { update(); });
}

DapQmlModelAutoServerList::DapQmlModelAutoServerList (DapAbstractServerList *a_serverList)
  : _abstractServerList (a_serverList)
  , _listType (_abstractServerList->type())
{
  if (_listType == Type::ServerList)
    connect (_serverList,       &QAbstractItemModel::modelReset, this, [this] { update(); });

  else if (_listType == Type::SortedServerList)
    connect (_sortedServerList, &QAbstractItemModel::modelReset, this, [this] { update(); });

  else
    qFatal(__PRETTY_FUNCTION__);
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelAutoServerList::setLocation (const QString &a_location)
{
  _location = a_location;
  update();
}

template<class T>
void _collectGeneralLocations (T &a_list, QSet <QString> &a_dest)
{
  for (auto &server : a_list)
    a_dest << server.name().left (server.name().indexOf ('.'));
}

template<class T>
void _fillAutoServerData (T &a_list, DapServerInfo &a_regionServer, const QString &a_location)
{
  for (const auto &server : a_list)
    {
      if (a_regionServer.name() == server.name().left (server.name().indexOf ('.'))
          && server.location() != a_location)
        {
          QString tmp  = a_regionServer.name();
          a_regionServer = server;
          a_regionServer.setName (tmp);
          break;
        }
    }
}

template<class T>
bool _bringAutoserverUp (T &a_list, DapServerList &a_autoServers, DapServerInfo &a_regionServer, const QString &a_location)
{
  for (const auto &server : a_list)
    {
      if (server.location() != a_location)
        {
          a_regionServer = server;
          a_regionServer.setName ("Auto");
          a_autoServers.move (a_autoServers.indexOf (a_regionServer), 0);
          return true;
        }
    }
  return false;
}

void DapQmlModelAutoServerList::update()
{
  QSet <QString> generalLocation;

  beginResetModel();

  /* add general auto servers */
  if (_autoServers.isEmpty())
    {
      generalLocation << "Auto";

      switch (_listType)
        {
        case Type::ServerList:        _collectGeneralLocations (*_serverList, generalLocation);
        case Type::SortedServerList:  _collectGeneralLocations (*_sortedServerList, generalLocation);
        default:
          _autoServers.clear();
          endResetModel();
          return;
        }

//      for (auto &server : _serverList)
//        generalLocation << server.name().left (server.name().indexOf ('.'));

      for (auto &location : qAsConst (generalLocation))
        {
          DapServerInfo item;
          item.setName (location);
          _autoServers.append (item);
        }
    }

  /* fill auto servers data */
  for (auto &regionServer : _autoServers)
    {
      switch (_listType)
        {
        case Type::ServerList:        _fillAutoServerData (*_serverList, regionServer, _location);
        case Type::SortedServerList:  _fillAutoServerData (*_sortedServerList, regionServer, _location);
        default:
          endResetModel();
          return;
        }

//      for (const auto &server : _serverList)
//        {
//          if (regionServer.name() == server.name().left (server.name().indexOf ('.'))
//              && server.location() != _location)
//            {
//              QString tmp  = regionServer.name();
//              regionServer = server;
//              regionServer.setName (tmp);
//              break;
//            }
//        }
    }

  /* sort result list */
  //_autoServers.sortByPing();
  qSort (_autoServers);

  /* top "Auto" server */
  for (auto &regionServer : _autoServers)
    {
      if (regionServer.name() != "Auto")
        continue;

      switch (_listType)
        {
        case Type::ServerList:        _bringAutoserverUp (*_serverList, _autoServers, regionServer, _location);
        case Type::SortedServerList:  _bringAutoserverUp (*_sortedServerList, _autoServers, regionServer, _location);
        default:
          _autoServers.clear();
          endResetModel();
          return;
        }

//      for (const auto &server : _serverList)
//        {
//          if (server.location() != _location)
//            {
//              regionServer = server;
//              regionServer.setName ("Auto");
//              _autoServers.move (_autoServers.indexOf (regionServer), 0);
//              break;
//            }
//        }
    }

  endResetModel();
}

const DapServerInfo &DapQmlModelAutoServerList::at (int a_index) const
{
  return _autoServers.at (a_index);
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelAutoServerList::rowCount (const QModelIndex &parent) const
{
  return _autoServers.rowCount (parent);
}

QVariant DapQmlModelAutoServerList::data (const QModelIndex &index, int role) const
{
  return _autoServers.data (index, role);
}

/*-----------------------------------------*/
