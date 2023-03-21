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
  QSet<QString> _allLocations;
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
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void _slotRowsInserted (const QModelIndex &, int start, int end);
  void _slotRowsMoved (const QModelIndex &, int sourceStart, int sourceEnd, const QModelIndex &, int destinationRow);
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
