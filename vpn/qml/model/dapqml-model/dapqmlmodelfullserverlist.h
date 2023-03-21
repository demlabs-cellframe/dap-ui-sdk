#ifndef DAPQMLMODELFULLSERVERLIST_H
#define DAPQMLMODELFULLSERVERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include "DapServerInfo.h"

/* DEFS */
class AbstractServerListModelBridge;

/****************************************//**
 * @brief full server list model
 *
 * Model provides list of servers combined
 * with auto servers
 *
 * @ingroup groupUiModels
 * @date 23.02.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelFullServerList : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int current READ current WRITE setCurrent NOTIFY currentChanged)
  Q_PROPERTY (int size READ size NOTIFY sizeChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  class ConstIterator
  {
  protected:
    const DapQmlModelFullServerList *p;
    int i;
  public:
    ConstIterator();
    ConstIterator (int n, const DapQmlModelFullServerList *p);
    ConstIterator (const ConstIterator &o);
    const DapServerInfo &operator*() const;
    const DapServerInfo *operator->() const;
    const DapServerInfo &operator[] (int j) const;
    bool operator== (const ConstIterator &o) const;
    bool operator!= (const ConstIterator &o) const;
    bool operator< (const ConstIterator &other) const;
    bool operator<= (const ConstIterator &other) const;
    bool operator> (const ConstIterator &other) const;
    bool operator>= (const ConstIterator &other) const;
    ConstIterator &operator++();
    ConstIterator operator++ (int);
    ConstIterator &operator--();
    ConstIterator operator-- (int);
    ConstIterator &operator+= (int j);
    ConstIterator &operator-= (int j);
    ConstIterator operator+ (int j) const;
    ConstIterator operator- (int j) const;
    friend inline ConstIterator operator+ (int j, ConstIterator k) { return k + j; }
    int operator- (ConstIterator j) const;
    operator const DapServerInfo *() const;
    int internalIndex() const;
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
  AbstractServerListModelBridge *m_bridge;
  QHash<int, QByteArray> _roleNamesMap;
  int m_current;
  QVector<QMetaObject::Connection> _conn;
  struct
  {
    int autoServer;
    int server;
    int full;
  } _size;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  DapQmlModelFullServerList();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelFullServerList *instance();
  const AbstractServerListModelBridge *bridge() const;
  /// @note will delete old bridge, if set
  void setBridge (AbstractServerListModelBridge *a_newBridge);
  Q_INVOKABLE int size() const;
  Q_INVOKABLE int current() const;
  Q_INVOKABLE void setCurrent (int a_newCurrentServer);
  Q_INVOKABLE QVariant value (int a_row, const QString &a_name);
  const DapServerInfo &currentServer() const;
  const DapServerInfo &at (int a_index) const;
  ConstIterator begin() const;
  ConstIterator end() const;
  int indexOfName (const QString &a_name) const;
protected:
  void _getSizes();
  void _getRoles();
  void _getCurrent();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void currentChanged();
  void sizeChanged();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELFULLSERVERLIST_H
