#ifndef DAPQMLMODELNODENETWORKSLIST_H
#define DAPQMLMODELNODENETWORKSLIST_H

/* INCLUDES */

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include "dapqmlmodelnodeproxybase.h"

/* DEFINES */

class QQmlEngine;
class QJSEngine;

/****************************************//**
 * @brief node networks list model
 * @ingroup groupUiModels
 * @date 27.02.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelNodeNetworksList : public QAbstractListModel, public DapQmlModelNodeProxyBase::Bridge
{
  Q_OBJECT

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
  explicit DapQmlModelNodeNetworksList (QObject *a_parent = nullptr);
  ~DapQmlModelNodeNetworksList() override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* basic methods */

  static DapQmlModelNodeNetworksList *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);
  void refresh();

  /* fields */

  int size() const;
  int currentIndex() const;
  bool setCurrentIndex (int a_index);
  bool isIndexed() const;
  bool filterAcceptsRow (int a_row, const QString &a_filter) const;
  const QString &network() const;

  /* new methods */

  /**
   * @brief Automatically select default network based on developer mode
   * @return true if network was selected, false otherwise
   */
  Q_INVOKABLE bool selectDefaultNetwork();

  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
protected:
  int rowCount (const QModelIndex &parent = QModelIndex()) const override;
  int columnCount (const QModelIndex &parent = QModelIndex()) const override;
  QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

private:
  void _modelReset();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCurrentIndexChanged();
  /// @}

};

/*-----------------------------------------*/
#endif // DAPQMLMODELNODENETWORKSLIST_H
