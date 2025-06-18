#ifndef DAPSERVERLIST_H
#define DAPSERVERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QQmlEngine>

#include "DapServerInfo.h"

/* DEFS */
class DapServerList;
class DapSortedServerList;

/****************************************//**
 * @brief abstract server list model
 *
 * Basiacly just a bunch of static methods.
 * Nothing special.
 *
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapAbstractServerList
{
  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static const QHash<int, QByteArray> &serverRoleNames();
  static const QHash<QString, QString> &countryMap();
  /// @}
};

/****************************************//**
 * @brief server list model
 *
 * Wrapper and also a model for DapServerInfoList
 *
 * Instance can be controled by user, but also
 * provides singleton instance
 *
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapServerList
  : public QAbstractListModel
  , public DapAbstractServerList
{
  Q_OBJECT

  friend class DapSortedServerList;

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
protected:
  typedef DapServerInfoList::iterator Iterator;
  typedef DapServerInfoList::const_iterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapServerInfoList m_list;
  int m_current;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapServerList();
  DapServerList (const DapServerInfoList &a_src);
  DapServerList (const DapServerList &a_src);
  DapServerList (DapServerList &&a_src);
  ~DapServerList() override;
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
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// get pointer to singleton instance
  static DapServerList *instance();

  int append (const DapServerInfo &a_server);
  int append (DapServerInfo &&a_server);
  void insert (int a_index, const DapServerInfo &a_server);
  void insert (int a_index, DapServerInfo &&a_server);
  void remove (int a_index);
  int size() const;
  bool empty() const;
  inline bool isEmpty() const { return empty(); }
  int indexOf (const DapServerInfo &a_item) const;
  /// search an item with provided name value
  int indexOfName (const QString &a_name) const;
  /// search an item with provided address value
  int indexOfAddress (const QString &a_address) const;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const;
  const DapServerInfo &last() const;
  DapServerInfo &at (int a_index);
  const DapServerInfo &at (int a_index) const;
  DapServerInfo value (int a_index) const;
  /// same as @ref value but can be accessed from QML
  Q_INVOKABLE QVariant qValue (int a_index) const;

  int current() const;
  void setCurrent (int a_index);
  /**
   * @brief get current server
   * @note if current is not set, will return a dummy instance
   */
  const DapServerInfo &currentServer() const;

  void move (int a_source, int a_dest);
  void clear();

  inline operator DapServerInfoList() const { return m_list; }
protected:
  /// search for iterator to return ints index
  int _iteratorIndex (Iterator &a_it) const;
  /// search for iterator to return ints index
  int _iteratorIndex (ConstIterator &a_it) const;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapServerList &operator = (const DapServerInfoList &a_src);
  DapServerList &operator = (const DapServerList &a_src);
  DapServerList &operator = (DapServerList &&a_src);
  DapServerList &operator<< (const DapServerInfo &a_server);
  DapServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

/****************************************//**
 * @brief sorted server list iterator
 *
 * Used to iterate items inside sorted server list
 *
 * Wrapps iterator object of indexes linked list.
 *
 * Allows to request items and their indexes
 *
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerListIterator
{
  friend class DapSortedServerListConstIterator;
public:
  typedef QList<int>::iterator Iterator;
protected:
  DapSortedServerList *p;
  Iterator i;
public:
  DapSortedServerListIterator();
  DapSortedServerListIterator (Iterator n, DapSortedServerList *p);
  DapSortedServerListIterator (const DapSortedServerListIterator &o);
  DapServerInfo &operator*();
  DapServerInfo *operator->();
  DapServerInfo &operator[] (int j);
  bool operator== (const DapSortedServerListIterator &o) const;
  bool operator!= (const DapSortedServerListIterator &o) const;
  bool operator< (const DapSortedServerListIterator &other) const;
  bool operator<= (const DapSortedServerListIterator &other) const;
  bool operator> (const DapSortedServerListIterator &other) const;
  bool operator>= (const DapSortedServerListIterator &other) const;
  DapSortedServerListIterator &operator++();
  DapSortedServerListIterator operator++ (int);
  DapSortedServerListIterator &operator--();
  DapSortedServerListIterator operator-- (int);
  DapSortedServerListIterator &operator+= (int j);
  DapSortedServerListIterator &operator-= (int j);
  DapSortedServerListIterator operator+ (int j) const;
  DapSortedServerListIterator operator- (int j) const;
  friend inline DapSortedServerListIterator operator+ (int j, DapSortedServerListIterator k) { return k + j; }
  int operator- (DapSortedServerListIterator j) const;
  operator DapServerInfo *();
  operator Iterator() const;
  int internalIndex() const;
  bool isNull() const;
};

/****************************************//**
 * @brief sorted server list const iterator
 *
 * Used to iterate items inside sorted server list
 *
 * Wrapps iterator object of indexes linked list.
 *
 * Allows to request items and their indexes
 *
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerListConstIterator
{
  friend class DapSortedServerListIterator;
public:
  typedef QList<int>::const_iterator ConstIterator;
protected:
  const DapSortedServerList *p;
  ConstIterator i;
public:
  DapSortedServerListConstIterator();
  DapSortedServerListConstIterator (ConstIterator n, const DapSortedServerList *p);
  DapSortedServerListConstIterator (const DapSortedServerListConstIterator &o);
  explicit DapSortedServerListConstIterator (const DapSortedServerListIterator &o);
  const DapServerInfo &operator*() const;
  const DapServerInfo *operator->() const;
  const DapServerInfo &operator[] (int j) const;
  bool operator== (const DapSortedServerListConstIterator &o) const;
  bool operator!= (const DapSortedServerListConstIterator &o) const;
  bool operator< (const DapSortedServerListConstIterator &other) const;
  bool operator<= (const DapSortedServerListConstIterator &other) const;
  bool operator> (const DapSortedServerListConstIterator &other) const;
  bool operator>= (const DapSortedServerListConstIterator &other) const;
  DapSortedServerListConstIterator &operator++();
  DapSortedServerListConstIterator operator++ (int);
  DapSortedServerListConstIterator &operator--();
  DapSortedServerListConstIterator operator-- (int);
  DapSortedServerListConstIterator &operator+= (int j);
  DapSortedServerListConstIterator &operator-= (int j);
  DapSortedServerListConstIterator operator+ (int j) const;
  DapSortedServerListConstIterator operator- (int j) const;
  friend inline DapSortedServerListConstIterator operator+ (int j, DapSortedServerListConstIterator k) { return k + j; }
  int operator- (DapSortedServerListConstIterator j) const;
  operator const DapServerInfo *() const;
  operator ConstIterator() const;
  int internalIndex() const;
  bool isNull() const;
};

/****************************************//**
 * @brief sorted server list model
 *
 * Wrapper and also a model for DapServerList.
 *
 * Used to store DapServerInfo instances
 * sorted by their ping value.
 *
 * Most of methods from this class mirrors
 * methods from @ref DapServerList
 *
 * @ingroup groupUiModels
 * @date 28.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerList
  : public QAbstractListModel
  , public DapAbstractServerList
{
  Q_OBJECT

  friend class DapSortedServerListIterator;
  friend class DapSortedServerListConstIterator;

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
protected:
  typedef DapSortedServerListIterator Iterator;
  typedef DapSortedServerListConstIterator ConstIterator;
  enum OperationType { Inserted, Removed };
public:
  struct IndexData
  {
    int index;
    QString name;
  };
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// unsorted list
  DapServerList _list;
  /// sorted indexes that points to items from unsorted list
  QList<int> _sortedIndexes;
  /// used for debug purposes
  QPair<IndexData, IndexData> m_sortIndexData;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapSortedServerList();
  ~DapSortedServerList() override;
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
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// get pointer to singleton instance
  static DapSortedServerList *instance();

  int append (const DapServerInfo &a_server);
  int append (DapServerInfo &&a_server);
  void insert (int a_index, const DapServerInfo &a_server);
  void insert (int a_index, DapServerInfo &&a_server);
  void remove (int a_index);
  int size() const;
  bool empty() const;
  inline bool isEmpty() const { return empty(); }
  int indexOf (const DapServerInfo &a_item) const;
  /// search an item with provided name value
  int indexOfName (const QString &a_name) const;
  /// search an item with provided address value
  int indexOfAddress (const QString &a_address) const;
  /// search an item with provided order hash value
  int indexOfOrderHash (const QString &a_hash) const;
  void erase (Iterator it);
  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;
  const DapServerInfo &first() const;
  const DapServerInfo &last() const;
  DapServerInfo &at (int a_index);
  const DapServerInfo &at (int a_index) const;
  DapServerInfo value (int a_index) const;
  /// same as @ref value but can be accessed from QML
  Q_INVOKABLE QVariant qValue (int a_index) const;

  int current() const;
  void setCurrent (int a_index);
  /**
   * @brief get current server
   * @note if current is not set, will return a dummy instance
   */
  const DapServerInfo &currentServer() const;

  void clear();
  /// sort all items from scratch
  inline void update() { _sort(); }
  /// sort only provided indexes
  void update (const QList<int> &a_indexes);
  /// get sorted items indexes
  const QList<int> &getSortedIndexes() const;
  QPair<IndexData, IndexData> sortIndexData() const;

  operator DapServerList() const;
  operator DapServerInfoList() const;
protected:
  void _sort();
  // int _appendServerIndex (const DapServerInfo &a_server, int a_index);
//  /// figure out where is the place for provided server
//  /// @return index where to put server, or -1 if you should append it to the end
//  int _beginInsertServer (const DapServerInfo &a_server) const;
//  /// finish by inserting or appending provided server
//  void _endInsertServer (int a_index, int a_dest);
  void _increaseAllIndexes (int a_index);
  void _decreaseAllIndexes (int a_index);
  void _fixCurrent (int a_index, OperationType a_operationType);
  int _iteratorIndex (Iterator &a_it);
  int _iteratorIndex (ConstIterator &a_it);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapSortedServerList &operator<< (const DapServerInfo &a_server);
  DapSortedServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERVERLIST_H
