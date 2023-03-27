#ifndef DAPQMLMODELAUTOSERVERLIST_H
#define DAPQMLMODELAUTOSERVERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include "DapServerList.h"

/****************************************//**
 * @brief servers model list
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelAutoServerList : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
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
  /// set general location
  void setLocation (const QString &a_location);
  const DapSortedServerList &getList() const { return _autoServers; };
  int current() const;
  void setCurrent (int a_newCurrent);
protected:
  void _connectSignals();
  void _reset();
  void _collectLocations (DapSortedServerList *a_list);
  void _buildUpAutoList (DapSortedServerList *a_dest);
  void _updateCurrent (QString &a_oldCurrentName);
  void _updateCurrent();
  int _autoServerIndex (const QString &a_name) const;
  DapServerInfo *_autoServerByName (const QString &a_name, int *a_destIndex = nullptr);
  int _locationIndex (const QString &a_location) const;
  Location *_locationByName (const QString &a_location, int *a_destIndex = nullptr);
  bool _containsLocation (const QString &a_location) const;
  void _increaseLocationIndexes (int a_index);
  void _decreaseLocationIndexes (int a_index);
  void _updateAutoServer (const DapServerInfo &a_server, const Location &a_location);
  QString _serverLocation (const DapServerInfo &a_server) const;
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotRowsInserted (const QModelIndex &, int first, int last);
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
