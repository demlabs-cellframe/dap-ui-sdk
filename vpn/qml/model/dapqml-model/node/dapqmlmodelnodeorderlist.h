#ifndef DAPQMLMODELNODEORDERLIST_H
#define DAPQMLMODELNODEORDERLIST_H

/* INCLUDES */

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include "dapqmlmodelnodedefines.h"

/* DEFINES */

class QQmlEngine;
class QJSEngine;

/****************************************//**
 * @brief node orders list model
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelNodeOrderList : public QAbstractListModel
{
  Q_OBJECT

  friend class DapQmlModelOrderListProxyModel;

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  struct Private; Private *p;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelNodeOrderList (QObject *a_parent = nullptr);
  ~DapQmlModelNodeOrderList();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* basic methods */

  static DapQmlModelNodeOrderList *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);

  Q_INVOKABLE int length() const;
  Q_INVOKABLE int indexOf (const QString &a_location);

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_index);

  /* order list control */

  const DapQmlModelNode::OrderItem *currentOrder() const;
  void setOrderListData (const QJsonArray &a_list, bool notify = true);

protected:
  void _modelReset();
  void _setNetworksFeeRequestList (const QStringList &a_list);
  QString _dequeueNetworkFeeRequest();
  void _setNetworkFee (const QString &a_networkName, const QString &a_fee);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;

  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:

  /* fields */

  void sigCurrentIndexChanged();
  void sigNetworkChanged();
  void sigWalletChanged();
  void sigTokenChanged();
  void sigUnitChanged();
  void sigBalanceChanged();

  /* orders */

  void sigOrderSelected (QString a_hash); // chooseOrder
  void sigRequestNodeIPs (QString networkName, QJsonArray orderList);
  void sigOrderListLoaded();
  void sigOrderListAddressesUpdated();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetData (const QJsonArray &a_list);
  void slotWalletsDataUpdated();
  void slotSetOrderAddresses (const QJsonObject &a_list);
  /// @}
};

/****************************************//**
 * @brief orders list model filter proxy
 * @ingroup groupUiModels
 * @date 26.02.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelOrderListProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
    /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QString m_unit;
  qreal m_min, m_max;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelOrderListProxyModel();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /**
   * @brief setup filter rules
   * @param a_unit unit name
   * @param a_min minimum value
   * @param a_max maximum value
   * @note set -1 into min or|and max to disable it's filtering
   */
  Q_INVOKABLE void setRowFilter (const QString &a_unit, qreal a_min, qreal a_max);

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_value);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCurrentIndexChanged();
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  bool filterAcceptsRow (int sourceRow, const QModelIndex &sourceParent) const override;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELNODEORDERLIST_H
