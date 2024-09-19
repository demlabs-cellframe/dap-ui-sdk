#ifndef DAPCMDSENDBUGREPORT_H
#define DAPCMDSENDBUGREPORT_H

/* INCLUDES */

#include <QObject>
#include <QJsonValue>

#include "DapCmdClientAbstract.h"

/****************************************//**
 * @brief send bug report command
 * @date 20.06.19
 * @author Danil Martynenko
 *******************************************/

class DapCmdSendBugReport : public DapCmdClientAbstract
{
  Q_OBJECT

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
public:
  QString m_email;
  QString m_message;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit DapCmdSendBugReport (QObject *a_parent = nullptr);
  virtual ~DapCmdSendBugReport() override { }
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void sendBugReport(
    const QString &a_message,
    const QString &a_serial = "",
    const QString &a_attachFile = "",
    const QString &a_contactAddress = "");
  void cancelBugReport();

protected:
  void handleResult (const QJsonObject& a_result) override;
  void handleError (int, const QString&) override;
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigBugReportSent (const QString& a_bugReportNumber);
  /// @}
};

/*-----------------------------------------*/
#endif // DAPCMDSENDBUGREPORT_H
