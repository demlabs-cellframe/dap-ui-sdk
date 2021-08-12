#ifndef DAPCMDCLIENTABSTRACT_H
#define DAPCMDCLIENTABSTRACT_H

/* INCLUDES */
#include "DapCmdAbstract.h"

/****************************************//**
 * @brief client message handler
 *******************************************/

class DapCmdClientAbstract : public DapCmdAbstract
{
  /****************************************//**
   * @name VARS
   *******************************************/
  /// @{
protected:
  /// Child class can use full error object
  const QJsonObject *m_errorObject;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  DapCmdClientAbstract (DapJsonCmdType cmd, QObject *parent);
  virtual ~DapCmdClientAbstract() override;
  /// @}

  /****************************************//**
   * @name OVERRIDES
   *******************************************/
  /// @{
protected:
  virtual void handle (const QJsonObject *params) override;
public:
  void sendCmd (const QJsonObject *obj = Q_NULLPTR) override;
  /// @}

  /****************************************//**
   * @name VIRTUAL INTERFACE
   *******************************************/
  /// @{
public:
  virtual void handleResult (const QJsonObject &result) = 0;
  virtual void handleError (int code, const QString &message) = 0;
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDCLIENTABSTRACT_H
