#ifndef DAPQMLMODELAUTOSERVERLIST_H
#define DAPQMLMODELAUTOSERVERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include "DapServerList.h"

/****************************************//**
 * @brief auto servers model list
 *
 * Builds a server list based on
 * general locations provided by data from
 * installed server list
 *
 * Reacts on installed server list changes
 *
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelAutoServerList : public QAbstractListModel
{
  Q_OBJECT

  friend class _CurrentUpdater;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:

  class Location
  {
    QString m_name;
    int m_index;
  public:
    explicit Location (const QString &a_src, int a_index = -1);
    explicit Location (const Location &a_src);
    explicit Location (Location &&a_src);

    const QString &name() const;
    void setName (const QString &a_name);

    int index() const;
    void setIndex (int a_index);

    operator const QString&() const;
    operator int() const;

    Location &operator = (const QString &a_src);
    Location &operator = (const Location &a_src);
    Location &operator = (Location &&a_src);

    bool operator == (const QString &o) const;
    bool operator == (const Location &o) const;
    bool operator != (const Location &o) const;
    bool operator < (const Location &other) const;
    bool operator <= (const Location &other) const;
    bool operator > (const Location &other) const;
    bool operator >= (const Location &other) const;
  };

  class _CurrentUpdater
  {
    QString _oldName;
    DapQmlModelAutoServerList *_parent;
  public:
    _CurrentUpdater (const _CurrentUpdater &a_src) : _oldName (a_src._oldName), _parent (a_src._parent) {}
    _CurrentUpdater (_CurrentUpdater &&a_src) : _oldName (std::move (a_src._oldName)), _parent (a_src._parent) {}
    _CurrentUpdater (const QString &a_name, DapQmlModelAutoServerList *a_parent) : _oldName (a_name), _parent (a_parent) {}
    ~_CurrentUpdater() { update(); }
    void set (const QString &a_value) { _oldName = a_value; }
    void update (const QString &a_value) { _oldName = a_value; update(); }
    void update();
  };

  typedef DapSortedServerList::IndexData IndexData;

  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// link to global servers list
  DapSortedServerList *_serverList;
  /// auto servers list
  DapSortedServerList _autoServers;
  /// general location
  QString _userLocation;
  /// available locations
  QList<Location> _allLocations;
  /// current server
  int m_current;
  /// controls current update routine
  _CurrentUpdater _currentUpdater;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelAutoServerList();
  DapQmlModelAutoServerList (DapSortedServerList *a_serverList);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  int indexOf (const DapServerInfo &a_item) const;
  /// search an item with provided name value
  int indexOfName (const QString &a_name) const;
  /// search an item with provided address value
  int indexOfAddress (const QString &a_address) const;
  /// set user general location
  void setLocation (const QString &a_location);
  const DapSortedServerList &getList() const { return _autoServers; };
  int current() const;
  void setCurrent (int a_newCurrent);
  QPair<IndexData, IndexData> sortIndexData() const;
protected:
  void _connectSignals();
  void _reset();
  void _collectLocations (DapSortedServerList *a_list);
  void _buildUpAutoList (DapSortedServerList *a_dest);
//  void _updateCurrent (QString &a_oldCurrentName);
//  void _updateCurrent();
  int _autoServerIndex (const QString &a_name) const;
  DapServerInfo *_autoServerByName (const QString &a_name, int *a_destIndex = nullptr);
  int _locationIndex (const QString &a_location) const;
  Location *_locationByName (const QString &a_location, int *a_destIndex = nullptr);
  bool _containsLocation (const QString &a_location) const;
  void _increaseLocationIndexes (int a_index);
  void _decreaseLocationIndexes (int a_index);
  void _updateAutoServer (const DapServerInfo &a_server, int a_serverIndex, const Location &a_location);
  void _updateAutoServer (const DapServerInfo &a_server);
  QString _serverLocation (const DapServerInfo &a_server) const;
  int _appendNewAutoServer (DapServerInfo &&a_server);
  bool _containsUserLocation (const QString &a_value) const;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotRowsAboutToBeInserted (const QModelIndex &, int first, int last);
  void _slotRowsInserted (const QModelIndex &, int first, int last);
  void _slotRowsAboutToBeMoved (const QModelIndex &, int sourceStart, int sourceEnd, const QModelIndex &, int destinationRow);
  void _slotRowsMoved (const QModelIndex &, int sourceStart, int sourceEnd, const QModelIndex &, int destinationRow);
  void _slotRowsAboutToRemoved (const QModelIndex &, int first, int last);
  void _slotRowsRemoved (const QModelIndex &, int first, int last);
  void _slotModelReset();
  void _slotDataChanged (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
  void _slotLayoutChanged (const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELAUTOSERVERLIST_H
