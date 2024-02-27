#ifndef DAPQMLMODELNODENETWORKSLIST_H
#define DAPQMLMODELNODENETWORKSLIST_H

/* INCLUDES */

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

/* DEFINES */

class QQmlEngine;
class QJSEngine;

/****************************************//**
 * @brief node networks list model
 * @ingroup groupUiModels
 * @date 27.02.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelNodeNetworksList : public QAbstractListModel
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
  ~DapQmlModelNodeNetworksList();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:

  /* basic methods */

  static DapQmlModelNodeNetworksList *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);

  /* fields */

  Q_INVOKABLE int currentIndex() const;
  Q_INVOKABLE void setCurrentIndex (int a_index);

  Q_INVOKABLE const QString &name() const;
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
};

/*-----------------------------------------*/
#endif // DAPQMLMODELNODENETWORKSLIST_H
