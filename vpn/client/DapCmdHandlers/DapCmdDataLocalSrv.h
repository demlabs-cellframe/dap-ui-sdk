#ifndef DAPCMDDATALOCALSRV_H
#define DAPCMDDATALOCALSRV_H

/* INCLUDES */

#include "DapCmdServiceAbstract.h"
#include <QObject>

/****************************************//**
 * @brief Dap Local Data command
 * @date 30.09.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdDataLocalSrv : public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdDataLocalSrv (QObject *a_parent = nullptr);
  virtual ~DapCmdDataLocalSrv() override;
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  /// Process command.
  /// @param params Command parameters.
  void handle (const QJsonObject *a_params) override;
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /// send copy of value to client
  void sendValue (const QString &a_name, const QByteArray &a_data, const int a_msgId);
  /// send copy of all data to client
  void sendAllData (const QJsonObject &a_data, const int a_msgId, const QString &a_clientType);
  /// send remove value to client
  void sendRemove (const QString &a_name, const int a_msgId);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /// client requested a value
  void sigGetValue (QString a_name, const int a_msgId);
  /// client requested all data
  void sigGetAllData (const int a_msgId, QString a_clientType);
  /// client wants to write a value
  void sigSetValue (QString a_name, QByteArray a_data, const int a_msgId);
  /// client wants to remove a value
  void sigRemoveValue (QString a_name, const int a_msgId);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDDATALOCALSRV_H
