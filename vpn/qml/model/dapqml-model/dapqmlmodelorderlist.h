#ifndef DAPQMLMODELORDERLIST_H
#define DAPQMLMODELORDERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
class QQmlEngine;
class QJSEngine;
namespace OrderListModule { struct OrderItem; class ModuleInterface; }
namespace Controler { class QmlCtl; }

/****************************************//**
 * @brief orders model list
 * @ingroup groupUiModels
 * @date 18.07.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelOrderList : public QAbstractListModel
{
  Q_OBJECT

  friend class DapQmlModelOrderListProxyModel;
  friend class Controler::QmlCtl;

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Mode
  {
    Invalid,

    Orders,
    Networks,
    Wallets,
    Tokens,
    Units,
  };
  Q_ENUM (Mode)

  enum class FieldId : qint32
  {
    /* order flields */
    location,
    node_addr,
    price,
    priceShort,
    units,
    units_value,
    server,
    hash,
    ipAddress,

    /* name + value fields */
    name,
    value,
    misc,

    /* model fields */
    network,
    wallet,
  };


protected:
  struct DapQmlModelOrderListData;
  /// @}

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (Mode mode           READ mode         WRITE setMode         NOTIFY sigModeChanged)
  Q_PROPERTY (int currentIndex    READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  Q_PROPERTY (QString network     READ network      WRITE setNetwork      NOTIFY sigNetworkChanged)
  Q_PROPERTY (QString wallet      READ wallet       WRITE setWallet       NOTIFY sigWalletChanged)
  Q_PROPERTY (QString token       READ token        WRITE setToken        NOTIFY sigTokenChanged)
  Q_PROPERTY (QString unit        READ unit         WRITE setUnit         NOTIFY sigUnitChanged)
  Q_PROPERTY (QString balance     READ balance      WRITE setBalance      NOTIFY sigBalanceChanged)
  /// @}

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  DapQmlModelOrderListData *_data;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit DapQmlModelOrderList (QObject *parent = nullptr);
  ~DapQmlModelOrderList();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelOrderList *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);

  Q_INVOKABLE int length() const;
  Q_INVOKABLE int indexOf (const QString &a_location);

  Q_INVOKABLE Mode mode() const;
  Q_INVOKABLE void setMode (Mode a_value);

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_index);

  Q_INVOKABLE QString network() const;
  Q_INVOKABLE void setNetwork (const QString &a_value = QString());

  Q_INVOKABLE QString wallet() const;
  Q_INVOKABLE void setWallet (const QString &a_value = QString());

  Q_INVOKABLE QString token() const;
  Q_INVOKABLE void setToken (const QString &a_value = QString());

  Q_INVOKABLE QString unit() const;
  Q_INVOKABLE void setUnit (const QString &a_value = QString());

  Q_INVOKABLE QString balance() const;
  Q_INVOKABLE void setBalance (const QString &a_value = QString());

  /* fields resets for child items */

//  Q_INVOKABLE void onNetworkChange();
//  Q_INVOKABLE void onWalletChange();

  const OrderListModule::OrderItem *currentOrder() const;
  void setOrderListData (const QJsonArray &a_list, bool notify = true);
protected:
  void _modelReset();
  void _setNwtworksFeeRequestList (const QStringList &a_list);
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
  void sigModeChanged();
  void sigCurrentIndexChanged();
  void sigNetworkChanged();
  void sigWalletChanged();
  void sigTokenChanged();
  void sigUnitChanged();
  void sigBalanceChanged();

  void sigNetworkUpdated (QString a_value);   // chooseNetwork
  void sigWalletUpdated (QString a_value);    // chooseWallet
  void sigTokenUpdated (QString a_value);     // chooseToken
  void sigTokenValueUpdated (QString a_value); // setValue
  //void sigMaxPriceUpdated (QString a_value);  // setValue
  void sigUnitUpdated (QString a_value);      // setUnit
  void sigMaxValueUpdated (QString a_value);  // setMaxValueUnit
  void sigMinValueUpdated (QString a_value);  // setMinValueUnit
  void sigOrderSelected (QString a_hash);     // chooseOrder

  void sigRequestNodeIPs (const QString &networkName, const QJsonArray &orderList);
  void sigOrderListLoaded();
  void sigOrderListAddressesUpdated();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetOrderListData (const QJsonArray &a_list);
//  void slotSetWalletListData (const QHash<QString, QStringList> &a_walletData);
//  void slotSetNetworkListData (const QHash<QString, QStringList> &a_networkData);
//  void slotSetTokensListData (const QHash<QString, QString> &a_tokensData);
  void slotWalletsDataUpdated();
  void slotSetOrderAddresses (const QJsonObject &a_list);
  /// @}
};

/****************************************//**
 * @brief orders filter model list
 * @ingroup groupUiModels
 * @date 18.07.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelOrderListProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex    READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
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

/****************************************//**
 * @brief list module filter model list
 * @ingroup groupUiModels
 * @date 27.10.2023
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlListModuleProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT

  /****************************************//**
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
public:
  enum Mode
  {
    Invalid,

    Networks,
    Wallets,
    Tokens,
  };

  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
private:
  QString m_filter;
  OrderListModule::ModuleInterface *_module;
  QAbstractListModel *_moduleModel;
  mutable QHash<int,int> _indexMap;
  mutable int _indexMapCounter;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlListModuleProxyModel (Mode a_mode);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /**
   * @brief filter by comparing mist field with provided filter
   * @note set empty string to disable filtering
   */
  Q_INVOKABLE void setRowFilter (const QString &a_filter);

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_value);
protected:
  void _setup (OrderListModule::ModuleInterface *a_module, QAbstractListModel *a_model);
  void _printFilteredResult() const;
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
#endif // DAPQMLMODELORDERLIST_H
