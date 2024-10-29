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

  friend class _RowsUpdater;

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

  class Index
  {
    int _value;
    int _autoSize;
    bool _isAuto;

  public:
    Index();
    Index (int a_value, int a_autoSize, bool a_isAuto);
    Index (const Index &a_src);

    /// will return auto index, if auto or sorted index, if not auto
    int internal() const;
    /// will return full list index
    int value() const;
    bool isAuto() const;
    bool isSorted() const;

    /// @see value
    operator int();
  };

protected:
  enum SenderType
  {
    Invalid,
    Auto,
    List,
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
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
  /// @note set a_newCurrentServer to -2 to reapply current auto server if selected any
  Q_INVOKABLE void setCurrent (int a_newCurrentServer);
  Q_INVOKABLE bool isCurrentAuto() const;
  Q_INVOKABLE QVariant value (int a_row, const QString &a_name) const;
  const DapServerInfo &currentServer() const;
  const DapServerInfo &at (int a_index) const;
  ConstIterator begin() const;
  ConstIterator end() const;
  Index indexOfName (const QString &a_name) const;
  Index indexOfAddress (const QString &a_address) const;
  Index indexOfOrderHash (const QString &a_hash) const;
  void refresh();
protected:
  void _getSizes();
  void _getRoles();
  void _getCurrent();
  SenderType _getSenderType() const;
  void _attach (QAbstractListModel *a_model);
  void _detach();
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

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
protected slots:
  void rowsAboutToBeInserted (const QModelIndex &, int start, int end);
  void rowsInserted (const QModelIndex &, int start, int end);
  void rowsAboutToBeMoved (const QModelIndex &, int first, int last, const QModelIndex &, int dest);
  void rowsMoved (const QModelIndex &, int first, int last, const QModelIndex &, int dest);
  void rowsAboutToBeRemoved (const QModelIndex &, int first, int last);
  void rowsRemoved (const QModelIndex &, int first, int last);
  void modelAboutToBeReset ();
  void modelReset ();
  void dataChanged (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELFULLSERVERLIST_H
