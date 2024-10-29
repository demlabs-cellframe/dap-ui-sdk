/* INCLUDES */
#include "dapqmlmodelautoserverlist.h"

/* DEFS */

class _QMetatDisconnector
{
  QMetaObject::Connection _conn;
public:
  _QMetatDisconnector (QMetaObject::Connection a_connection) : _conn (a_connection) {}
  ~_QMetatDisconnector() { QObject::disconnect (_conn); }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelAutoServerList::DapQmlModelAutoServerList()
  : _serverList (DapSortedServerList::instance())
  , m_current (-1)
  , _currentUpdater (QString(), this)
{
  _reset();
  _connectSignals();
}

DapQmlModelAutoServerList::DapQmlModelAutoServerList (DapSortedServerList *a_serverList)
  : _serverList (a_serverList)
  , m_current (-1)
  , _currentUpdater (QString(), this)
{
  _reset();
  _connectSignals();
}

/********************************************
 * METHODS
 *******************************************/

int DapQmlModelAutoServerList::indexOf (const DapServerInfo &a_item) const
{
  int index = 0;
  for (auto i = _autoServers.begin(), e = _autoServers.end(); i != e; i++, index++)
    if (*i == a_item)
      return index;
  return -1;
}

int DapQmlModelAutoServerList::indexOfName (const QString &a_name) const
{
  int index = 0;
  for (auto i = _autoServers.begin(), e = _autoServers.end(); i != e; i++, index++)
    if (i->name() == a_name)
      return index;
  return -1;
}

int DapQmlModelAutoServerList::indexOfAddress (const QString &a_address) const
{
  int index = 0;
  for (auto i = _autoServers.begin(), e = _autoServers.end(); i != e; i++, index++)
    if (i->address() == a_address)
      return index;
  return -1;
}

void DapQmlModelAutoServerList::setLocation (const QString &a_location)
{
  if (_userLocation == a_location)
    return;

  _userLocation = a_location;
  _reset();
}

int DapQmlModelAutoServerList::current() const
{
  return m_current;
}

void DapQmlModelAutoServerList::setCurrent (int a_newCurrent)
{
  m_current = a_newCurrent;
  _autoServers.setCurrent (a_newCurrent);
}

QPair<DapQmlModelAutoServerList::IndexData, DapQmlModelAutoServerList::IndexData> DapQmlModelAutoServerList::sortIndexData() const
{
  return _autoServers.sortIndexData();
}

void DapQmlModelAutoServerList::_connectSignals()
{
  connect (_serverList, &QAbstractListModel::rowsAboutToBeInserted, this, &DapQmlModelAutoServerList::_slotRowsAboutToBeInserted);
  connect (_serverList, &QAbstractListModel::rowsInserted,          this, &DapQmlModelAutoServerList::_slotRowsInserted);
  connect (_serverList, &QAbstractListModel::rowsAboutToBeMoved,    this, &DapQmlModelAutoServerList::_slotRowsAboutToBeMoved);
  connect (_serverList, &QAbstractListModel::rowsMoved,             this, &DapQmlModelAutoServerList::_slotRowsMoved);
  connect (_serverList, &QAbstractListModel::rowsAboutToBeRemoved,  this, &DapQmlModelAutoServerList::_slotRowsAboutToRemoved);
  connect (_serverList, &QAbstractListModel::rowsRemoved,           this, &DapQmlModelAutoServerList::_slotRowsRemoved);
  connect (_serverList, &QAbstractListModel::modelReset,            this, &DapQmlModelAutoServerList::_slotModelReset);
  connect (_serverList, &QAbstractListModel::dataChanged,           this, &DapQmlModelAutoServerList::_slotDataChanged);
  connect (_serverList, &QAbstractListModel::layoutChanged,         this, &DapQmlModelAutoServerList::_slotLayoutChanged);
}

void DapQmlModelAutoServerList::_reset()
{
  QString oldCurrentName;
  if (m_current != -1)
    oldCurrentName  = _autoServers.at (m_current).name();

  beginResetModel();
  _collectLocations (_serverList);
  _buildUpAutoList (&_autoServers);
  _currentUpdater.update (oldCurrentName);//_updateCurrent (oldCurrentName);
  endResetModel();
}

void DapQmlModelAutoServerList::_collectLocations (DapSortedServerList *a_list)
{
  _allLocations.clear();
  for (const auto &server : *a_list)
    {
      auto locName  = _serverLocation (server);
      if (_locationByName (locName) == nullptr)
      _allLocations << Location (locName);
    }
  _allLocations.insert (0, Location ("Auto"));
}

void DapQmlModelAutoServerList::_buildUpAutoList (DapSortedServerList *a_dest)
{
  DapServerInfo bestServer;
  int bestServerIndex = -1;
  a_dest->clear();

  /* collect auto servers */
  for (auto &location : _allLocations)
    {
      /* find first */
      int serverIndex = 0;
      for (auto i = _serverList->cbegin(), e = _serverList->cend(); i != e; i++, serverIndex++)
        {
          auto serverName = i->name();
          /* add basic auto server */
          if (serverName.startsWith (location)
              && !_containsUserLocation (serverName)) // !serverName.contains (_userLocation))
            {
//              /* store best region server */
//              if (location == _userLocation)
//                {
//                  bestServer      = *i;
//                  bestServerIndex = serverIndex;
//                  bestServer.setName ("Auto");
//                }

              /* store */
              DapServerInfo item  = *i;
              item.setName (location);
              *a_dest << std::move (item);

              /* save index */
              location.setIndex (serverIndex);

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
    {
      a_dest->insert (0, bestServer);

#ifdef QT_DEBUG
      if (_allLocations.first().name() != "Auto")
        qFatal ("%s First item of allLocations is not \"Auto\"!", __PRETTY_FUNCTION__);
#endif // QT_DEBUG

      _allLocations[0].setIndex (bestServerIndex);
    }
}

//void DapQmlModelAutoServerList::_updateCurrent (QString &a_oldCurrentName)
//{
//  if (a_oldCurrentName.isEmpty())
//    return;

//  int newCurrent = -1, index = 0;

//  for (auto i = _autoServers.cbegin(), e = _autoServers.cend(); i != e; i++, index++)
//    {
//      if (i->name() != a_oldCurrentName)
//        continue;

//      newCurrent  = index;
//      break;
//    }

//  setCurrent (newCurrent);
//}

//void DapQmlModelAutoServerList::_updateCurrent()
//{
//  QString oldCurrentName;
//  if (m_current != -1)
//    oldCurrentName  = _autoServers.at (m_current).name();
//  _updateCurrent (oldCurrentName);
//}

int DapQmlModelAutoServerList::_autoServerIndex (const QString &a_name) const
{
  int index = 0;
  for (auto i = _autoServers.cbegin(), e = _autoServers.cend(); i != e; i++, index++)
    if (i->name() == a_name)
      return index;
  return -1;
}

DapServerInfo *DapQmlModelAutoServerList::_autoServerByName (const QString &a_name, int *a_destIndex)
{
  int index = 0;
  for (auto i = _autoServers.begin(), e = _autoServers.end(); i != e; i++, index++)
    if (i->name() == a_name)
      {
        if (a_destIndex)
          *a_destIndex = index;
        return i;
      }

  if (a_destIndex)
    *a_destIndex = -1;
  return nullptr;
}

int DapQmlModelAutoServerList::_locationIndex (const QString &a_location) const
{
  int index = 0;
  for (auto i = _allLocations.cbegin(), e = _allLocations.cend(); i != e; i++, index++)
    if (*i == a_location)
      return index;
  return -1;
}

DapQmlModelAutoServerList::Location *DapQmlModelAutoServerList::_locationByName (const QString &a_location, int *a_destIndex)
{
  int index = 0;
  for (auto i = _allLocations.begin(), e = _allLocations.end(); i != e; i++, index++)
    if (i->name() == a_location)
      {
        if (a_destIndex)
          *a_destIndex = index;
        return &*i;
      }

  if (a_destIndex)
    *a_destIndex = -1;
  return nullptr;
}

bool DapQmlModelAutoServerList::_containsLocation (const QString &a_location) const
{
  return _locationIndex (a_location) != -1;
}

void DapQmlModelAutoServerList::_increaseLocationIndexes (int a_index)
{
  for (auto i = _allLocations.begin(), e = _allLocations.end(); i != e; i++)
    if (*i >= a_index)
      i->setIndex (i->index() + 1);
}

void DapQmlModelAutoServerList::_decreaseLocationIndexes (int a_index)
{
  for (auto i = _allLocations.begin(); i != _allLocations.end(); i++)
    {
      /* decrease */
      if (*i >= a_index)
        i->setIndex (i->index() - 1);

//      /* remove if same */
//      else if (*i == a_index)
//        {
//          auto c = i;
//          if (i != _allLocations.begin())
//            i--;
//          _allLocations.erase (c);
//        }
    }
}

void DapQmlModelAutoServerList::_updateAutoServer (const DapServerInfo &a_server, int a_serverIndex, const DapQmlModelAutoServerList::Location &a_location)
{
  /* lambda */
  auto getAutoServer = [this] () -> DapServerInfo&
    {
      if (_autoServers.isEmpty() || _autoServers.first().name() != "Auto")
        {
          beginInsertRows (QModelIndex(), 0, 0);
          DapServerInfo as (QString(), "Auto", "", 0);
          _autoServers.insert (0, as);
          endInsertRows();
        }
      return _autoServers[0];
    };

  auto getAutoLocation  = [this] () -> Location&
    {
      if (_allLocations.isEmpty() || _allLocations.first().name() != "Auto")
        {
          Location newLoc {"Auto", -1};
          _allLocations.insert (0, std::move (newLoc));
        }
      return _allLocations[0];
    };

  /* vars */
  DapServerInfo &autoServer = getAutoServer();
  Location &autoLocation    = getAutoLocation();

#ifdef QT_DEBUG
  if (autoServer.name() != "Auto")
    qFatal ("%s Auto server is not first!", __PRETTY_FUNCTION__);
  if (autoLocation.name() != "Auto")
    qFatal ("%s Auto location is not first!", __PRETTY_FUNCTION__);
#endif // QT_DEBUG

//  /* if user location */
//  if (a_location == _userLocation)
//    //&& (autoServer.ping() > a_server.ping() || autoServer.ping() == -1))
//    {
//      autoServer  = a_server;
//      autoServer.setName ("Auto");
//      autoLocation.setIndex (a_serverIndex);

//      emit dataChanged (index (0), index (0));
//      return;
//    }

//  /* check auto value */
//  if (_userLocation.isEmpty()
//    && (autoServer.ping() > _serverList->first().ping() || autoServer.ping() == -1))
//    {
//      autoServer  = _serverList->first();
//      autoServer.setName ("Auto");
//      autoLocation.setIndex (0);

//      emit dataChanged (index (0), index (0));
//      return;
//  }

  /* set best auto server */
  for (const auto &server : qAsConst (*_serverList))
  {
    if (_containsUserLocation (server.name()))
      continue;

    if (autoServer.ping() > server.ping() || autoServer.ping() == -1)
      {
        autoServer  = server;
        autoServer.setName ("Auto");
        autoLocation.setIndex (0);

        emit dataChanged (index (0), index (0));
        return;
    }
  }
}

void DapQmlModelAutoServerList::_updateAutoServer (const DapServerInfo &a_server)
{
  /* reset auto server */
  DapServerInfo *autoServer = _autoServerByName ("Auto");
  *autoServer = DapServerInfo (QString(), "Auto", QString(), 0);
  autoServer->setPing (9999);

  /* get server location */
  Location *autoLoc = _locationByName ("Auto");

  /* find the best suitable server */
  int index = 0;
  for (auto i = _serverList->cbegin(), e = _serverList->cend(); i != e; i++, index++)
    {
      if (_containsUserLocation (i->name()))
        continue;

      if (i->ping() != -1
          && i->ping() < autoServer->ping()
          && i->address() != a_server.address())
        {
          *autoServer = *i;
          autoServer->setName ("Auto");
          autoLoc->setIndex (index);
          return;
        }
    }

  if (autoServer->ping() == 9999)
    autoServer->setPing (-1);
}

QString DapQmlModelAutoServerList::_serverLocation (const DapServerInfo &a_server) const
{
  int length = a_server.name().indexOf('.');
  return a_server.name().left (length);
}

int DapQmlModelAutoServerList::_appendNewAutoServer(DapServerInfo &&a_server)
{
  /* attach lambdas */
  _QMetatDisconnector _begin = connect (&_autoServers, &QAbstractListModel::rowsAboutToBeInserted,
                                        this, [this] (const QModelIndex &parent, int first, int last)
    {
      beginInsertRows (parent, first, last);
    });
  _QMetatDisconnector _end = connect (&_autoServers, &QAbstractListModel::rowsInserted,
                                      this, [this] (const QModelIndex &, int, int)
    {
      endInsertRows();
    });

  /* perform */
  return _autoServers.append (std::move (a_server));
}

bool DapQmlModelAutoServerList::_containsUserLocation(const QString &a_value) const
{
  if (_userLocation.isEmpty())
    return false;
  return a_value.contains (_userLocation);
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelAutoServerList::_slotRowsAboutToBeInserted (const QModelIndex &, int, int)
{
  /* update current */
  if (m_current != -1)
    _currentUpdater.set (_autoServers.at (m_current).name());
}

void DapQmlModelAutoServerList::_slotRowsInserted (const QModelIndex &, int first, int last)
{
  /*
   * -inserted server that is new location
   * -inserted server that replacing old location
   *
   * +update Auto server when needed
   * +update current
   */

  auto moveAutoServerToTheBeginning = [this]
    {
      QModelIndex dummy;

      /* get "Auto server pointer */
      int autoIndex;
      DapServerInfo *autoServerPtr  = _autoServerByName ("Auto", &autoIndex);

      /* check if exists */
      if (autoServerPtr == nullptr)
        {
          /* add auto server */
          beginInsertRows (dummy, 0, 0);
          _autoServers.insert (0, DapServerInfo (QString(), "Auto", QString(), 0));
          endInsertRows();
        }

      /* move back to the beginning */
      else
        if (autoIndex != 0)
          {
            DapServerInfo autoServerCopy = *autoServerPtr;
            beginMoveRows (dummy, autoIndex, autoIndex, dummy, 0);
            _autoServers.remove (autoIndex);
            _autoServers.insert (0, autoServerCopy);
            endMoveRows();
          }
    };

  QModelIndex dummy;
  for (int i = first; i <= last; i++)
  {
    /* get inserted server */
    const DapServerInfo &server = qAsConst (_serverList)->at (i);

    /* ignore user location ones */
    if (_containsUserLocation (server.name()))
      continue;

    /* vars */
    //int locationIndex;
    QString locationName  = _serverLocation (server);
    Location *location    = _locationByName (locationName);//, &locationIndex);

    /* check this is a new location */
    if (location == nullptr)
      {
        /* new location */
        Location newLoc (locationName, i);

        /* new server */
        DapServerInfo item  = server;
        item.setName (locationName);

        /* store */
        int index = _appendNewAutoServer (std::move (item));//_autoServers.append (std::move (item));

        /* fix "Auto" server pos */
        moveAutoServerToTheBeginning();

        /* increase indexes */
        _increaseLocationIndexes (i);

        /* add new location */
        _allLocations << newLoc;

        /* update */
        beginInsertRows (dummy, index, index);
        endInsertRows();

        _updateAutoServer (server, i, newLoc);
        _currentUpdater.update();//_updateCurrent();

        continue;
      }

    /* if index is the same, changes has to be made */
    if (location->index() >= i)
      {
        /* new server */
        DapServerInfo item  = server;
        item.setName (locationName);

        /* remove old */
        int oldIndex  = _autoServerIndex (locationName);

#ifdef QT_DEBUG
        if (oldIndex == -1)
          qFatal ("%s Auto server not found! \"%s\"", __PRETTY_FUNCTION__, locationName.toUtf8().data());
#endif // QT_DEBUG

        beginRemoveRows (dummy, oldIndex, oldIndex);
        _autoServers.remove (oldIndex);
        endRemoveRows();

        /* add new */
        /*int newIndex  = */_appendNewAutoServer (std::move (item));//_autoServers.append (std::move (item));

        /* fix "Auto" server pos */
        moveAutoServerToTheBeginning();

        /* increase indexes */
        _increaseLocationIndexes (i);

        /* update location index */
        location->setIndex (i);

        /* update */
        _updateAutoServer (server, i, *location);
        _currentUpdater.update();//_updateCurrent();

        continue;
      }

    /* update location index */
    _increaseLocationIndexes (i);
  }

  //_reset();
}

void DapQmlModelAutoServerList::_slotRowsAboutToBeMoved (const QModelIndex &, int, int, const QModelIndex &, int)
{
  /* update current */
  if (m_current != -1)
    _currentUpdater.set (_autoServers.at (m_current).name());
}

void DapQmlModelAutoServerList::_slotRowsMoved (const QModelIndex &, int first, int last, const QModelIndex &, int dest)
{
  /*
   * -check first moved ping
   * -check last moved ping
   *
   * +update Auto server when needed
   * +update current
   */

  /* collect end result idnexes */
  QList<int> indexes;
  for (int i = first; i <= last; i++)
    indexes << dest + i;
  indexes << first;

  /* check all indexes data */
  QModelIndex dummy;
  QSet<QString> updatedServers;
  for (const auto index : qAsConst (indexes))
    {
      /* get inserted server */
      const DapServerInfo &server = qAsConst (_serverList)->at (index);

      /* ignore user location ones */
      if (_containsUserLocation (server.name()))
        continue;

      /* vars */
      //int locationIndex;
      QString locationName  = _serverLocation (server);
      Location *location    = _locationByName (locationName);//, &locationIndex);

#ifdef QT_DEBUG
      if (location == nullptr)
        qFatal ("%s Location not found! \"%s\"", __PRETTY_FUNCTION__, locationName.toUtf8().data());
#endif // QT_DEBUG

      /* update location server */
      for (const auto &bestServer : qAsConst (*_serverList))
        {
          /* skip update, if it is still actual */
          if (bestServer.address() == server.address())
            goto _slotRowsMovedContinue;

          /* vars */
          auto bestServerLocation = _serverLocation (bestServer);

          /* skip servers that is already updated */
          if (updatedServers.contains (bestServerLocation))
            continue;

          /* skip other locations */
          if (bestServerLocation != locationName)
            continue;

          /* store the name of the updated server */
          updatedServers << locationName;

          /* update auto server */
          int autoIndex   = _autoServerIndex (locationName);
          DapServerInfo item = bestServer;
          item.setName (locationName);

#ifdef QT_DEBUG
          if (autoIndex == -1)
            qFatal ("%s Auto server not found! \"%s\"", __PRETTY_FUNCTION__, locationName.toUtf8().data());
#endif // QT_DEBUG

          /* replace old auto server with sorting applied */
          beginRemoveRows (dummy, autoIndex, autoIndex);
          _autoServers.remove (autoIndex);
          endRemoveRows();

          /*int newIndex  = */_appendNewAutoServer (std::move (item));//_autoServers.append (std::move (item));

          /* check if auto is moved */
          if (_autoServers.first().name() != "Auto")
            {
              /* move back to the beginning */
              int autoIndex;
              DapServerInfo autoServerCopy = *_autoServerByName ("Auto", &autoIndex);
              beginMoveRows (dummy, autoIndex, autoIndex, dummy, 0);
              _autoServers.remove (autoIndex);
              _autoServers.insert (0, autoServerCopy);
              endMoveRows();
            }

          /* update location index */
          location->setIndex (index);

          /* update */
          _updateAutoServer (server, index, *location);
          _currentUpdater.update();//_updateCurrent();
        }
_slotRowsMovedContinue:
      continue;
    }

  //_reset();
}

void DapQmlModelAutoServerList::_slotRowsAboutToRemoved (const QModelIndex &, int first, int last)
{
  /*
   * -removed auto location server
   * -removed auto location server and it was the only location server
   * -removed server that is not in any location
   *
   * +update Auto server when needed
   * +update current
   */

  auto isMoreThanOneLocationLeft = [this] (const QString &a_locationName)
    {
      bool found = false;
      for (auto i = _serverList->cbegin(), e = _serverList->cend(); i != e; i++)
        {
          if (_serverLocation (*i) != a_locationName)
            continue;

          if (!found)
            found = true;
          else
            return true;
        }
      return false;
    };

  QModelIndex dummy;
  for (int i = first; i <= last; i++)
    {
      /* get inserted server */
      const DapServerInfo &server = qAsConst (_serverList)->at (i);

      /* ignore user location ones */
      if (_containsUserLocation (server.name()))
        continue;

      /* vars */
      int autoIndex, locationIndex;
      QString locationName      = _serverLocation (server);
      Location *location        = _locationByName (locationName, &locationIndex);
      DapServerInfo *autoServer = _autoServerByName (locationName, &autoIndex);

      /* flags */
      bool removedAuto      = (autoServer) ? autoServer->address() == server.address() : false;
      bool lastAuto         = !isMoreThanOneLocationLeft (locationName);

      /* if not last, update to most best */
      if (removedAuto && !lastAuto && autoServer)
        {
          /* find the best available */
          int newIndex = 0;
          for (auto i = _serverList->cbegin(), e = _serverList->cend(); i != e; i++, newIndex++)
            {
              /* skip same item */
              if (i->address() == autoServer->address())
                continue;

              /* skip wrong loc */
              if (_serverLocation (*i) != locationName)
                continue;

              /* store */
              *autoServer = *i;
              autoServer->setName (locationName);
              location->setIndex (newIndex);

              /* decrease indexes */
              _decreaseLocationIndexes (newIndex);

              /* update */
              emit dataChanged (index (newIndex), index (newIndex));

              if (_userLocation.isEmpty())
                _updateAutoServer (server);
              else
                _updateAutoServer (*i, newIndex, *location);

              if (m_current != -1)
                _currentUpdater.set (_autoServers.at (m_current).name());
              //_updateCurrent();

              break;
            }

          continue;
        }

      /* if last auto is removed */
      if (removedAuto && lastAuto)
        {
          /* update */
          if (m_current != -1)
            _currentUpdater.set (_autoServers.at (m_current).name());

          /* decrease indexes */
          _decreaseLocationIndexes (i);

          /* remove */
          beginRemoveRows (dummy, autoIndex, autoIndex);
          _autoServers.remove (autoIndex);
          _allLocations.removeAt (locationIndex);
          endRemoveRows();

          /* update */
          if (locationName == _userLocation || _userLocation.isEmpty())
            _updateAutoServer (server);

          continue;
        }

      /* removed server in the middle of the list */
      if (!removedAuto)
        {
          /* decrease indexes */
          _decreaseLocationIndexes (i);

          /* update */
          if (m_current != -1)
            _currentUpdater.set (_autoServers.at (m_current).name());
          //_updateCurrent();

          continue;
        }

#ifdef QT_DEBUG
        qFatal ("%s The loop is not supposed to end here!", __PRETTY_FUNCTION__);
#endif // QT_DEBUG
    }

  //_reset();
}

void DapQmlModelAutoServerList::_slotRowsRemoved (const QModelIndex &, int, int)
{
  /* update current */
  _currentUpdater.update();

  //_reset();
}

void DapQmlModelAutoServerList::_slotModelReset()
{
  _reset();
}

void DapQmlModelAutoServerList::_slotDataChanged (const QModelIndex &, const QModelIndex &, const QVector<int> &)
{
  //_reset();
}

void DapQmlModelAutoServerList::_slotLayoutChanged (const QList<QPersistentModelIndex> &, QAbstractItemModel::LayoutChangeHint)
{
  //_reset();
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

DapQmlModelAutoServerList::Location::Location (const QString &a_src, int a_index)
{
  operator = (a_src);
  m_index = a_index;
}

DapQmlModelAutoServerList::Location::Location (const DapQmlModelAutoServerList::Location &a_src)
{
  operator = (a_src);
}

DapQmlModelAutoServerList::Location::Location (DapQmlModelAutoServerList::Location &&a_src)
{
  operator = (std::move (a_src));
}

const QString &DapQmlModelAutoServerList::Location::name() const
{
  return m_name;
}

void DapQmlModelAutoServerList::Location::setName (const QString &a_name)
{
  m_name  = a_name;
}

int DapQmlModelAutoServerList::Location::index() const
{
  return m_index;
}

void DapQmlModelAutoServerList::Location::setIndex (int a_index)
{
  m_index = a_index;
}

DapQmlModelAutoServerList::Location::operator const QString &() const
{
  return m_name;
}

DapQmlModelAutoServerList::Location::operator int() const
{
  return m_index;
}

DapQmlModelAutoServerList::Location &DapQmlModelAutoServerList::Location::operator = (const QString &a_src)
{
  m_name  = a_src;
  m_index = -1;
  return *this;
}

DapQmlModelAutoServerList::Location &DapQmlModelAutoServerList::Location::operator = (const Location &a_src)
{
  if (this != &a_src)
    {
      m_name  = a_src.m_name;
      m_index = a_src.m_index;
    }

  return *this;
}

DapQmlModelAutoServerList::Location &DapQmlModelAutoServerList::Location::operator = (Location &&a_src)
{
  if (this != &a_src)
    {
      m_name  = std::move (a_src.m_name);
      m_index = a_src.m_index;

      a_src.m_index = -1;
    }

  return *this;
}

bool DapQmlModelAutoServerList::Location::operator == (const QString &o) const
{
  return m_name == o;
}

bool DapQmlModelAutoServerList::Location::operator == (const DapQmlModelAutoServerList::Location &o) const
{
  return m_index == o.m_index;
}

bool DapQmlModelAutoServerList::Location::operator != (const DapQmlModelAutoServerList::Location &o) const
{
  return m_index != o.m_index;
}

bool DapQmlModelAutoServerList::Location::operator < (const DapQmlModelAutoServerList::Location &other) const
{
  return m_index < other.m_index;
}

bool DapQmlModelAutoServerList::Location::operator <= (const DapQmlModelAutoServerList::Location &other) const
{
  return m_index <= other.m_index;
}

bool DapQmlModelAutoServerList::Location::operator > (const DapQmlModelAutoServerList::Location &other) const
{
  return m_index > other.m_index;
}

bool DapQmlModelAutoServerList::Location::operator >= (const DapQmlModelAutoServerList::Location &other) const
{
  return m_index >= other.m_index;
}

/*-----------------------------------------*/

void DapQmlModelAutoServerList::_CurrentUpdater::update()
{
  if (_oldName.isEmpty())
    return;

  int newCurrent = -1, index = 0, oldCurrent = _parent->current();
  auto &_autoServers  = _parent->_autoServers;

  for (auto i = _autoServers.cbegin(), e = _autoServers.cend(); i != e; i++, index++)
    {
      if (i->name() != _oldName)
        continue;

      newCurrent  = index;
      break;
    }

  _parent->setCurrent (newCurrent);

#ifdef QT_DEBUG
  qDebug("%s : old [n:%s,i:%d] new [i:%d]",
         "DapQmlModelAutoServerList::_CurrentUpdater::update",
         _oldName.toUtf8().data(),
         oldCurrent,
         newCurrent);
#endif // QT_DEBUG

  _oldName.clear();
}

/*-----------------------------------------*/
