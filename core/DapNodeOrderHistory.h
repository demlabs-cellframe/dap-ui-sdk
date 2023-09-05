#ifndef DAPNODEORDERHISTORY_H
#define DAPNODEORDERHISTORY_H

/* INCLUDES */
#include <QAbstractListModel>
#include "DapNodeOrderInfo.h"

/****************************************//**
 * @brief node order history
 * @date 05.09.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapNodeOrderHistory : public QAbstractListModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
  typedef DapNodeOrderInfoList::Iterator Iterator;
  typedef DapNodeOrderInfoList::ConstIterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  DapNodeOrderInfoList _list;
  int m_currentIndex;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapNodeOrderHistory (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapNodeOrderHistory *instance();

  const DapNodeOrderInfo &at (int a_index) const;
  int size() const;

  void append (const DapNodeOrderInfo &a_value);
  void append (DapNodeOrderInfo &&a_value);
  void prepend (const DapNodeOrderInfo &a_value);
  void prepend (DapNodeOrderInfo &&a_value);
  void insert (int a_index, const DapNodeOrderInfo &a_value);
  void insert (int a_index, DapNodeOrderInfo &&a_value);
  void insert (Iterator a_index, const DapNodeOrderInfo &a_value);
  void insert (Iterator a_index, DapNodeOrderInfo &&a_value);

  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;

  int currentIndex() const;
  void setCurrentIndex (int a_value);
  const DapNodeOrderInfo &current() const;

  void load();
  void save() const;

  void itemUpdated (int a_index);
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
  void currentIndexChanged();
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  DapNodeOrderInfo &operator[] (int a_index);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODEORDERHISTORY_H
