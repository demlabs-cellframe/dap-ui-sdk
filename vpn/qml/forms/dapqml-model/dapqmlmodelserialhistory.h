#ifndef DAPQMLMODELSERIALHISTORY_H
#define DAPQMLMODELSERIALHISTORY_H

/* INCLUDES */
#include <QAbstractTableModel>
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
#endif // DAPQMLMODELSERIALHISTORY_H
