#ifndef DAPSERVERLIST_H
#define DAPSERVERLIST_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QLinkedList>

#include "DapServerInfo.h"

/* DEFS */
class DapServerList;
class DapSortedServerList;

/****************************************//**
 * @brief abstract server list model
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

template<class Iterator, class ConstIterator>
class DapAbstractServerList
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
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapAbstractServerList() {}
  virtual ~DapAbstractServerList() = 0;
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
  virtual int append (const DapServerInfo &a_server) = 0;
  virtual int append (DapServerInfo &&a_server) = 0;
  virtual void insert (int a_index, const DapServerInfo &a_server) = 0;
  virtual void insert (int a_index, DapServerInfo &&a_server) = 0;
  virtual void remove (int a_index) = 0;
  virtual int size() const = 0;
  virtual bool empty() const = 0;
  inline bool isEmpty() { return empty(); }
  virtual int indexOf (const DapServerInfo &a_item) const = 0;
  virtual void erase (Iterator it) = 0;
  virtual Iterator begin() = 0;
  virtual ConstIterator begin() const = 0;
  virtual ConstIterator cbegin() const = 0;
  virtual Iterator end() = 0;
  virtual ConstIterator end() const = 0;
  virtual ConstIterator cend() const = 0;
  virtual const DapServerInfo &first() const = 0;
  virtual const DapServerInfo &last() const = 0;
  virtual DapServerInfo &at (int a_index) = 0;
  virtual const DapServerInfo &at (int a_index) const = 0;
  virtual DapServerInfo value (int a_index) const = 0;
  /// return server item copy as QVariantMap
  Q_INVOKABLE virtual QVariant qValue (int a_index) const = 0;

  virtual int current() const = 0;
  virtual void setCurrent (int a_index) = 0;
  virtual const DapServerInfo &currentServer() const = 0;

  virtual void move (int a_source, int a_dest) = 0;
  virtual void clear() = 0;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index)                { return at (a_index); }
  const DapServerInfo &operator[] (int a_index) const    { return at (a_index); }
  DapAbstractServerList &operator<< (const DapServerInfo &a_server)  { append (a_server); return *this; }
  DapAbstractServerList &operator<< (DapServerInfo &&a_server)       { append (std::move (a_server)); return *this; }
  /// @}
};

/****************************************//**
 * @brief server list model
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapServerList
  : public QAbstractListModel
  , public DapAbstractServerList<DapServerInfoList::iterator, DapServerInfoList::const_iterator>
{
  Q_OBJECT

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
  ~DapServerList() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapServerList *instance();

  int append (const DapServerInfo &a_server) override;
  int append (DapServerInfo &&a_server) override;
  void insert (int a_index, const DapServerInfo &a_server) override;
  void insert (int a_index, DapServerInfo &&a_server) override;
  void remove (int a_index) override;
  int size() const override;
  bool empty() const override;
  int indexOf (const DapServerInfo &a_item) const override;
  void erase (Iterator it) override;
  Iterator begin() override;
  ConstIterator begin() const override;
  ConstIterator cbegin() const override;
  Iterator end() override;
  ConstIterator end() const override;
  ConstIterator cend() const override;
  const DapServerInfo &first() const override;
  const DapServerInfo &last() const override;
  DapServerInfo &at (int a_index) override;
  const DapServerInfo &at (int a_index) const override;
  DapServerInfo value (int a_index) const override;
  Q_INVOKABLE QVariant qValue (int a_index) const override;

  int current() const override;
  void setCurrent (int a_index) override;
  const DapServerInfo &currentServer() const override;

  void move (int a_source, int a_dest) override;
  void clear() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapServerList &operator<< (const DapServerInfo &a_server);
  DapServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

class DapSortedServerListIterator
{
  friend class DapSortedServerListConstIterator;
  typedef QLinkedList<int>::iterator Iterator;
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
  friend DapSortedServerListIterator operator+ (int j, DapSortedServerListIterator k) { return k + j; }
  int operator- (DapSortedServerListIterator j) const;
  operator DapServerInfo *();
};

class DapSortedServerListConstIterator
{
  friend class DapSortedServerListIterator;
  typedef QLinkedList<int>::const_iterator ConstIterator;
  DapSortedServerList *p;
  ConstIterator i;
public:
  DapSortedServerListConstIterator();
  DapSortedServerListConstIterator (ConstIterator n, DapSortedServerList *p);
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
  friend DapSortedServerListConstIterator operator+ (int j, DapSortedServerListConstIterator k) { return k + j; }
  int operator- (DapSortedServerListConstIterator j) const;
  operator const DapServerInfo *() const;
};

/****************************************//**
 * @brief sorted server list model
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapSortedServerList
  : public QAbstractListModel
  , public DapAbstractServerList<DapSortedServerListIterator, DapSortedServerListConstIterator>
{
  Q_OBJECT

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  typedef DapSortedServerListIterator Iterator;
  typedef DapSortedServerListConstIterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapServerList m_list;
  QLinkedList<int> _sortedIndexes;
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
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapSortedServerList *instance();

  int append (const DapServerInfo &a_server) override;
  int append (DapServerInfo &&a_server) override;
  void insert (int a_index, const DapServerInfo &a_server) override;
  void insert (int a_index, DapServerInfo &&a_server) override;
  void remove (int a_index) override;
  int size() const override;
  bool empty() const override;
  int indexOf (const DapServerInfo &a_item) const override;
  void erase (Iterator it) override;
  Iterator begin() override;
  ConstIterator begin() const override;
  ConstIterator cbegin() const override;
  Iterator end() override;
  ConstIterator end() const override;
  ConstIterator cend() const override;
  const DapServerInfo &first() const override;
  const DapServerInfo &last() const override;
  const DapServerInfo &at (int a_index) const override;
  DapServerInfo value (int a_index) const override;
  Q_INVOKABLE QVariant qValue (int a_index) const override;

  int current() const override;
  void setCurrent (int a_index) override;
  const DapServerInfo &currentServer() const override;

  void move (int a_source, int a_dest) override;
  void clear() override;

  /// sort servers list by ping */
  void sortByPing();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent) const Q_DECL_OVERRIDE;
  QVariant data (const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapServerInfo &operator[] (int a_index);
  const DapServerInfo &operator[] (int a_index) const;
  DapServerList &operator<< (const DapServerInfo &a_server);
  DapServerList &operator<< (DapServerInfo &&a_server);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPSERVERLIST_H
