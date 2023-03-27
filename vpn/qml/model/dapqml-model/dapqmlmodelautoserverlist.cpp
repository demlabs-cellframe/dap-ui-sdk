/* INCLUDES */
#include "dapqmlmodelautoserverlist.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelAutoServerList::DapQmlModelAutoServerList()
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
  connect (_serverList, &QAbstractListModel::rowsInserted,          this, &DapQmlModelAutoServerList::_slotRowsInserted);
  connect (_serverList, &QAbstractListModel::rowsMoved,             this, &DapQmlModelAutoServerList::_slotRowsMoved);
  connect (_serverList, &QAbstractListModel::rowsAboutToBeRemoved,  this, &DapQmlModelAutoServerList::_slotRowsAboutToRemoved);
  //connect (_serverList, &QAbstractListModel::rowsRemoved,           this, &DapQmlModelAutoServerList::_slotRowsRemoved);
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
  _updateCurrent (oldCurrentName);
  endResetModel();
}

void DapQmlModelAutoServerList::_collectLocations (DapSortedServerList *a_list)
{
  _allLocations.clear();
  for (const auto &server : *a_list)
    _allLocations << Location (_serverLocation (server));
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
          /* add basic auto server */
          if (i->name().startsWith (location))
            {
              /* store best region server */
              if (location == _userLocation)
                {
                  bestServer      = *i;
                  bestServerIndex = serverIndex;
                  bestServer.setName ("Auto");
                }

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

void DapQmlModelAutoServerList::_updateCurrent()
{
  QString oldCurrentName;
  if (m_current != -1)
    oldCurrentName  = _autoServers.at (m_current).name();
  _updateCurrent (oldCurrentName);
}

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
  for (auto i = _allLocations.begin(), e = _allLocations.end(); i != e; i++)
    {
      /* decrease */
      if (*i > a_index)
        i->setIndex (i->index() - 1);

      /* remove if same */
      else if (*i == a_index)
        {
          auto c = i;
          i--;
          _allLocations.erase (c);
        }
    }
}

void DapQmlModelAutoServerList::_updateAutoServer (const DapServerInfo &a_server, const DapQmlModelAutoServerList::Location &a_location)
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

  /* vars */
  DapServerInfo &autoServer = getAutoServer();
  Location &autoLocation    = _allLocations[0];

#ifdef QT_DEBUG
  if (autoServer.name() != "Auto")
    qFatal ("%s Auto server is not first!", __PRETTY_FUNCTION__);
  if (autoLocation.name() != "Auto")
    qFatal ("%s Auto location is not first!", __PRETTY_FUNCTION__);
#endif // QT_DEBUG

  /* if user location */
  if (a_location == _userLocation)
    {
      autoServer  = a_server;
      autoServer.setName ("Auto");

      emit dataChanged (index (0), index (0));
      return;
    }

  /* check auto value */
  if (autoServer.ping() > _serverList->first().ping())
    {
      autoServer  = _serverList->first();
      autoServer.setName ("Auto");

      emit dataChanged (index (0), index (0));
      return;
    }
}

QString DapQmlModelAutoServerList::_serverLocation (const DapServerInfo &a_server) const
{
  int length = a_server.name().indexOf('.');
  return a_server.name().left (length);
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelAutoServerList::_slotRowsInserted (const QModelIndex &, int first, int last)
{
  /*
   * -inserted server that is new location
   * -inserted server that replacing old location
   *
   * +update Auto server when needed
   * +update current
   */

  QModelIndex dummy;
  for (int i = first; i <= last; i++)
  {
    /* get inserted server */
    const DapServerInfo &server = qAsConst (_serverList)->at (i);

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
        int index = _autoServers.append (std::move (item));

        /* increase indexes */
        _increaseLocationIndexes (index);

        /* add new location */
        _allLocations << newLoc;

        /* update */
        beginInsertRows (dummy, index, index);
        endInsertRows();

        _updateAutoServer (server, newLoc);
        _updateCurrent();

        continue;
      }

    /* if index is the same, changes has to be made */
    if (location->index() == i)
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
        int newIndex  = _autoServers.append (std::move (item));
        beginInsertRows (dummy, newIndex, newIndex);
        endInsertRows();

        /* update location index */
        location->setIndex (newIndex);

        /* increase indexes */
        _increaseLocationIndexes (newIndex);

        /* update */
        _updateAutoServer (server, *location);
        _updateCurrent();

        continue;
      }

//    /* if index is the same, changes has to be made */
//    if (location->index() == i)
//      {
//        /* get auto server */
//        int serverIndex;
//        DapServerInfo *autoServer  = _autoServerByName (*location, &serverIndex);

//#ifdef QT_DEBUG
//        if (autoServer == nullptr)
//          qFatal ("%s Location is not presented inside auto server list! \"%s\"", __PRETTY_FUNCTION__, locationName.toUtf8().data());
//#endif // QT_DEBUG

//        /* update auto server */
//        *autoServer   = server;
//        autoServer->setName (locationName);

//        /* update location index */
//        location->setIndex (i);

//        /* increase indexes */
//        _increaseLocationIndexes (i);

//        /* update */
//        emit dataChanged (index (serverIndex), index (serverIndex));
//      }
  }

  //_reset();
}

void DapQmlModelAutoServerList::_slotRowsMoved (const QModelIndex &, int, int, const QModelIndex &, int)
{
  _reset();
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

  auto isMoreThanOneLocationLeft = [this] (const Location &a_location)
    {
      bool found = false;
      for (auto i = _serverList->cbegin(), e = _serverList->cend(); i != e; i++)
        {
          if (_serverLocation (*i) != a_location)
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

      /* vars */
      int autoIndex;//int locationIndex;
      QString locationName      = _serverLocation (server);
      Location *location        = _locationByName (locationName);//, &locationIndex);
      DapServerInfo *autoServer = _autoServerByName (locationName, &autoIndex);

      /* flags */
      bool removedAuto      = (autoServer) ? autoServer->address() == server.address() : false;
      bool lastAuto         = isMoreThanOneLocationLeft (*location);

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

              _updateAutoServer (*i, *location);
              _updateCurrent();

              break;
            }

          continue;
        }

      /* if last auto is removed */
      if (removedAuto && lastAuto)
        {
          /* decrease indexes */
          _decreaseLocationIndexes (i);

          /* remove */
          beginRemoveRows (dummy, autoIndex, autoIndex);
          _autoServers.remove (autoIndex);
          endRemoveRows();

          /* update */
          _updateAutoServer (server, *location);
          _updateCurrent();

          continue;
        }

      /* removed server in the middle of the list */
      if (!removedAuto)
        {
          /* decrease indexes */
          _decreaseLocationIndexes (i);

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
  //_reset();
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
