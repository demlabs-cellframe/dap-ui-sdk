#ifndef DAPNODETRANSACTIONHISTORY_H
#define DAPNODETRANSACTIONHISTORY_H

/* INCLUDES */
#include <QAbstractListModel>
#include "DapNodeOrderInfo.h"

/****************************************//**
 * @brief node order history
 * @date 05.09.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapNodeTransactionHistory : public QAbstractListModel
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
  struct Transaction
  {
    DapNodeOrderInfo info;
    QString ipAddress;
    QString network;
    QString wallet;
    QString token;
    QString tokenValue; // user payed
    QString unit;
    QString unitValue;
    QString priceValue; // per portion
    QString portions;

    QString totalValue;
    QDateTime created;
    bool isSigned;

    operator QJsonObject() const { return toJsonObject(); }
    void fromJson (const QJsonObject &a_obj);
    QJsonObject toJsonObject() const;
  };

  typedef QList<Transaction> OrderList;

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
  explicit DapNodeTransactionHistory (QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapNodeTransactionHistory *instance();

  const Transaction &at (int a_index) const;
  int size() const;

  void append (const DapNodeTransactionHistory::Transaction &a_value);
  void append (DapNodeTransactionHistory::Transaction &&a_value);
  void prepend (const DapNodeTransactionHistory::Transaction &a_value);
  void prepend (DapNodeTransactionHistory::Transaction &&a_value);
  void insert (int a_index, const DapNodeTransactionHistory::Transaction &a_value);
  void insert (int a_index, DapNodeTransactionHistory::Transaction &&a_value);
  void insert (Iterator a_index, const DapNodeTransactionHistory::Transaction &a_value);
  void insert (Iterator a_index, DapNodeTransactionHistory::Transaction &&a_value);

  Iterator begin();
  ConstIterator begin() const;
  ConstIterator cbegin() const;
  Iterator end();
  ConstIterator end() const;
  ConstIterator cend() const;

  int currentIndex() const;
  void setCurrentIndex (int a_value);
  const Transaction &current() const;
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
  DapNodeTransactionHistory::Transaction &operator[] (int a_index);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPNODETRANSACTIONHISTORY_H
