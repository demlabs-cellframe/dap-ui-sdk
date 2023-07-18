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
   * @name PROPERTIES
   *******************************************/
  /// @{
  Q_PROPERTY (int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY sigCurrentIndexChanged)
  /// @}

  /****************************************//**
   * @name DEFS
   *******************************************/
  /// @{
protected:
  struct DapQmlModelOrderListData;
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
  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_index);
  Q_INVOKABLE int indexOf (const QString &a_location);
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
  void sigCurrentIndexChanged();
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetData (const QMap<QString, QVariant> &a_orderListData);
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
