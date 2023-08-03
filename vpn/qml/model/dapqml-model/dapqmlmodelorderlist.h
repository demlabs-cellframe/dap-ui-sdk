#ifndef DAPQMLMODELORDERLIST_H
#define DAPQMLMODELORDERLIST_H

/* INCLUDES */
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
class QQmlEngine;
class QJSEngine;

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
  };
  Q_ENUM (Mode)

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
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetOrderListData (const QJsonArray &a_list);
  void slotSetWalletListData (const QHash<QString, QStringList> &a_walletData);
  void slotSetNetworkListData (const QHash<QString, QStringList> &a_networkData);
  void slotSetTokensListData (const QHash<QString, QString> &a_tokensData);
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
   * @name VARS
   *******************************************/
  /// @{
private:
  QString m_filter;
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
  Q_INVOKABLE void updateCheckedIndex (const QString &a_checkedName);
  Q_INVOKABLE void setRowFilter (const QString a_filter);
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
