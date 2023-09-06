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
public:
  struct Order
  {
    DapNodeOrderInfo info;
    QString wallet;
    QString network;
    QString token;
    QString value;
    QString unit;
    bool isSigned;

    operator QJsonObject() const { return toJsonObject(); }
    void fromJson (const QJsonObject &a_obj);
    QJsonObject toJsonObject() const;
  };

  typedef QList<Order> OrderList;

  typedef OrderList::Iterator Iterator;
  typedef OrderList::ConstIterator ConstIterator;
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  OrderList _list;
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

  const Order &at (int a_index) const;
  int size() const;

  void append (const DapNodeOrderHistory::Order &a_value);
  void append (DapNodeOrderHistory::Order &&a_value);
  void prepend (const DapNodeOrderHistory::Order &a_value);
  void prepend (DapNodeOrderHistory::Order &&a_value);
  void insert (int a_index, const DapNodeOrderHistory::Order &a_value);
  void insert (int a_index, DapNodeOrderHistory::Order &&a_value);
  void insert (Iterator a_index, const DapNodeOrderHistory::Order &a_value);
  void insert (Iterator a_index, DapNodeOrderHistory::Order &&a_value);

  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;

  int currentIndex() const;
  void setCurrentIndex (int a_value);
  const Order &current() const;
  int indexOf (const QString &a_orderHash) const;

  void load();
  void save() const;

  /// sends dataChanged signal
  void itemUpdated (int a_index);
protected:
  void _delayedSave() const;
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
  DapNodeOrderHistory::Order &operator[] (int a_index);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODEORDERHISTORY_H
