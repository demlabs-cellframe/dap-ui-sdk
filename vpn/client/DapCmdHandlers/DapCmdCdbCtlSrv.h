#ifndef DapCmdCdbCtlSrv_H
#define DapCmdCdbCtlSrv_H

/* INCLUDES */
#include <QObject>
#include "DapCmdServiceAbstract.h"

/****************************************//**
 * @brief Get/Set cdb servers list
 * @date 26.10.2022
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdCdbCtlSrv : public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  static const QString s_fieldCdb;
  static const QString s_fieldValue;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdCdbCtlSrv(QObject *parent = nullptr);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void sendCmdGetList (const QString &a_value);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  void handle (const QJsonObject* a_params) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCdbList (const QString a_value);
  /// @}
};

/*-----------------------------------------*/
#endif // DapCmdCdbCtlSrv_H
