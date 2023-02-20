#ifndef DAPSERVERLIST_H
#define DAPSERVERLIST_H

/* INCLUDES */
#include <QAbstractTableModel>

#include "DapServerInfo.h"

/****************************************//**
 * @brief servers model list
 * @ingroup groupUiModels
 * @date 11.03.2021
 * @author Mikhail Shilenko
 *******************************************/

class DapServerList : public QAbstractListModel
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

  typedef DapServerInfoList::iterator iterator;
  typedef DapServerInfoList::const_iterator const_iterator;
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
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapServerList *instance();

  int append (const DapServerInfo &a_server);
  int append (DapServerInfo &&a_server);
  void insert (int a_index, const DapServerInfo &a_server);
  void insert (int a_index, DapServerInfo &&a_server);
  void remove (int a_index);
  int size() const;
  bool empty() const;
  bool isEmpty() { return empty(); }
  int indexOf (const DapServerInfo &a_item) const;
  void erase (iterator it);
  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  const DapServerInfo &first() const;
  const DapServerInfo &last() const;
  const DapServerInfo &at (int a_index) const;
  DapServerInfo value (int a_index) const;
  /// return server item copy as QVariantMap
  Q_INVOKABLE QVariant qValue (int a_index) const;

  int current() const;
  void setCurrent (int a_index);
  const DapServerInfo &currentServer() const;

  void move (int a_source, int a_dest);

  /// sort servers list by ping */
  void sort();
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
  DapServerInfo &operator[](int a_index);
  const DapServerInfo &operator[](int a_index) const;
  DapServerList &operator<<(const DapServerInfo &a_server);
  DapServerList &operator<<(DapServerInfo &&a_server);
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
#endif // DAPSERVERLIST_H
