#ifndef DAPQMLMODELSERIALHISTORY_H
#define DAPQMLMODELSERIALHISTORY_H

/* INCLUDES */
#include <QAbstractTableModel>
#include <QTimer>
#include "DapCmdDataLocal.h"
class QQmlEngine;
class QJSEngine;

/****************************************//**
 * @brief bug reports model list
 * @ingroup groupUiModels
 * @date 09.03.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapQmlModelSerialHistory : public QAbstractTableModel
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapQmlModelSerialHistory(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static DapQmlModelSerialHistory *instance();
  Q_INVOKABLE static QObject *singletonProvider (QQmlEngine *engine, QJSEngine *scriptEngine);
  Q_INVOKABLE int length() const;
  Q_INVOKABLE void attachAnimation (QObject *a_copyButton);
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
  void slotSetup(DapCmdDataLocal * pCmdDataLocal);
  void update();
private:
  DapCmdDataLocal * m_pCmdDataLocal;
  /// @}
};

class _DapQmlModelSerialHistoryCopyButtonAnimation : public QObject
{
  Q_OBJECT
  QScopedPointer<QTimer> m_timer;

public:
  _DapQmlModelSerialHistoryCopyButtonAnimation (QObject *a_copyButton);

protected slots:
  void slotStart();
  void slotFinish();
};

/*-----------------------------------------*/
#endif // DAPQMLMODELSERIALHISTORY_H
