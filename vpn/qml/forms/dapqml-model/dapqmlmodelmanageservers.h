#ifndef DAPQMLMODELMANAGESERVERS_H
#define DAPQMLMODELMANAGESERVERS_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QSharedPointer>
class QQmlEngine;
class QJSEngine;
class AbstractServerManager;

/****************************************//**
 * @brief manage servers model list
 * @ingroup groupUiModels
 * @date 15.07.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelManageServers : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapQmlModelManageServers();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelManageServers *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);
  Q_INVOKABLE int length() const;
  void installManager (QSharedPointer<AbstractServerManager> a_manager);

  void fillDummyList();
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
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotSetup();
  /// @}
};

/*-----------------------------------------*/
#endif // DAPQMLMODELMANAGESERVERS_H
