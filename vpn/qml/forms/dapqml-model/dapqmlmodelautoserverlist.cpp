/* INCLUDES */
#include "dapqmlmodelautoserverlist.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelAutoServerList::DapQmlModelAutoServerList ()
  : _serverList (*DapServerList::instance())
{

}

DapQmlModelAutoServerList::DapQmlModelAutoServerList (DapServerList &a_serverList)
  : _serverList (a_serverList)
{

}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelAutoServerList::setLocation (const QString &a_location)
{
  _location = a_location;
  update();
}

void DapQmlModelAutoServerList::update()
{
  QSet <QString> generalLocation;

  /* add general auto servers */
  if (_autoServers.isEmpty())
    {
      generalLocation << "Auto";

      for (auto &server : _serverList)
        generalLocation << server.name().left (server.name().indexOf ('.'));

      for (auto &location : qAsConst(generalLocation))
        {
          DapServerInfo item;
          item.setName (location);
          _autoServers.append (item);
        }
    }

  /* servers list copy with cut name */
  for (auto &regionServer : _autoServers)
    {
      for (const auto &server : _serverList)
        {
          if (regionServer.name() == server.name().left (server.name().indexOf ('.'))
              && server.location() != _location)
            {
              QString tmp  = regionServer.name();
              regionServer = server;
              regionServer.setName (tmp);
              break;
            }
        }
    }

  /* sort result list */
  _autoServers.sort();

  /* top "Auto" server */
  for (auto &region_server : _autoServers)
    {
      for (const auto &server : _serverList)
        {
          if (region_server.name() == "Auto"
              && server.location() != _location)
            {
              region_server = server;
              region_server.setName ("Auto");
              _autoServers.move (_autoServers.indexOf (region_server), 0);
              break;
            }
        }
    }
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
