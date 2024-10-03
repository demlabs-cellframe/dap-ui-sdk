#ifndef DAPCMDDATALOCAL_H
#define DAPCMDDATALOCAL_H

/* INCLUDES */

#include "DapCmdServiceAbstract.h"
#include <QObject>

/****************************************//**
 * @brief Dap Local Data command
 * @date 30.09.2024
 * @author Mikhail Shilenko
 *******************************************/

class DapCmdDataLocal : public DapCmdServiceAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  QString m_clientType;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdDataLocal (QObject *a_parent = nullptr);
  virtual ~DapCmdDataLocal() override;
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
  const QString &clientTypeName() const;
  void setClientTypeName (const QString &a_clientType);
  /// request value from serive
  void requestValue (const QString &a_name, const int a_msgId);
  /// request all data from service
  void requestAllData (const int a_msgId = 0);
  /// send value to service
  void sendValue (const QString &a_name, const QByteArray &a_value, const int a_msgId);
  /// send remove value to service
  void sendRemove (const QString &a_name, const int a_msgId);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  /// service sent value
  void sigGotValue (QString a_name, QByteArray a_data, const int a_msgId);
  /// service sent all data
  void sigGotAllData (QJsonObject a_data, const int a_msgId, QString a_clientType);
  /// service sent remove value
  void sigGotRemove (QString a_name, const int a_msgId);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDDATALOCAL_H
