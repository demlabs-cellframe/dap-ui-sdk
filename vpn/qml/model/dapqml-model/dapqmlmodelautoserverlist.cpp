/* INCLUDES */
#include "dapqmlmodelautoserverlist.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelAutoServerList::DapQmlModelAutoServerList ()
  : _serverList (DapSortedServerList::instance())
  , m_current (-1)
{
  _reset();
  _connectSignals();
}

DapQmlModelAutoServerList::DapQmlModelAutoServerList (DapSortedServerList *a_serverList)
  : _serverList (a_serverList)
  , m_current (-1)
{
  _reset();
  _connectSignals();
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelAutoServerList::setLocation (const QString &a_location)
{
  _userLocation = a_location;
}

int DapQmlModelAutoServerList::current() const
{
  return m_current;
}

void DapQmlModelAutoServerList::setCurrent (int a_newCurrent)
{
  m_current = a_newCurrent;
}

void DapQmlModelAutoServerList::_connectSignals()
{
  connect (_serverList, &QAbstractListModel::rowsInserted,  this, &DapQmlModelAutoServerList::_slotRowsInserted);
  connect (_serverList, &QAbstractListModel::rowsMoved,     this, &DapQmlModelAutoServerList::_slotRowsMoved);
  connect (_serverList, &QAbstractListModel::rowsRemoved,   this, &DapQmlModelAutoServerList::_slotRowsRemoved);
  connect (_serverList, &QAbstractListModel::modelReset,    this, &DapQmlModelAutoServerList::_slotModelReset);
  connect (_serverList, &QAbstractListModel::dataChanged,   this, &DapQmlModelAutoServerList::_slotDataChanged);
  connect (_serverList, &QAbstractListModel::layoutChanged, this, &DapQmlModelAutoServerList::_slotLayoutChanged);
}

void DapQmlModelAutoServerList::_reset()
{
  QString oldCurrentName;
  if (m_current != -1)
    oldCurrentName  = _autoServers.at (m_current).name();

  beginResetModel();
  _collectLocations (_serverList);
  _buildUpAutoList (&_autoServers);
  _updateCurrent (oldCurrentName);
  endResetModel();
}

void DapQmlModelAutoServerList::_collectLocations (DapSortedServerList *a_list)
{
  _allLocations.clear();
  for (const auto &server : *a_list)
    _allLocations << server.name().left (server.name().indexOf('.'));
}

void DapQmlModelAutoServerList::_buildUpAutoList (DapSortedServerList *a_dest)
{
  DapServerInfo bestServer;
  a_dest->clear();
  for (const auto &location : qAsConst (_allLocations))
    {
      /* find first */
      for (const auto &server : *_serverList)
        {
          /* add basic auto server */
          if (server.name().startsWith (location))
            {
              /* store best region server */
              if (location == _userLocation)
                {
                  bestServer  = server;
                  bestServer.setName ("Auto");
                }

              /* store */
              DapServerInfo item  = server;
              item.setName (location);
              *a_dest << std::move (item);
              goto _resetCollectContinue;
            }
        }

_resetCollectContinue:
      continue;
    }

  /* check if best region exists */
  if (bestServer.address().isEmpty()
      && !a_dest->isEmpty())
    {
      bestServer  = a_dest->first();
      bestServer.setName ("Auto");
    }

  /* insert to the beginning */
  if (!bestServer.address().isEmpty())
    a_dest->insert (0, bestServer);
}

void DapQmlModelAutoServerList::_updateCurrent (QString &a_oldCurrentName)
{
  int newCurrent = -1, index = 0;

  for (auto i = _autoServers.cbegin(), e = _autoServers.cend(); i != e; i++, index++)
    {
      if (i->name() != a_oldCurrentName)
        continue;

      newCurrent  = index;
      break;
    }

  setCurrent (newCurrent);
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelAutoServerList::_slotRowsInserted (const QModelIndex &, int, int)
{
  _reset();
}

void DapQmlModelAutoServerList::_slotRowsMoved (const QModelIndex &, int, int, const QModelIndex &, int)
{
  _reset();
}

void DapQmlModelAutoServerList::_slotRowsRemoved (const QModelIndex &, int, int)
{
  _reset();
}

void DapQmlModelAutoServerList::_slotModelReset()
{
  _reset();
}

void DapQmlModelAutoServerList::_slotDataChanged (const QModelIndex &, const QModelIndex &, const QVector<int> &)
{
  _reset();
}

void DapQmlModelAutoServerList::_slotLayoutChanged (const QList<QPersistentModelIndex> &, QAbstractItemModel::LayoutChangeHint)
{
  _reset();
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
