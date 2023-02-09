#ifndef DAPCMDSENDBUGREPORT_H
#define DAPCMDSENDBUGREPORT_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdClientAbstract.h"

class DapCmdSendBugReport : public DapCmdClientAbstract
{
  Q_OBJECT
  QString m_email;
  QString m_message;
  bool m_waitingForResponse;

public:
  explicit DapCmdSendBugReport (QObject *a_parent = nullptr);
  virtual ~DapCmdSendBugReport() override { }

public:
  void sendBugReport(
    const QString &a_message,
    const QString &a_serial = "",
    const QString &a_attachFile = "",
    const QString &a_emailAddress = "");
  void cancelBugReport();

protected:
  void handleResult (const QJsonObject& a_result) override;
  void handleError (int, const QString&) override;

signals:
  void sigBugReportSent (const QString& a_bugReportNumber);
};

#endif // DAPCMDSENDBUGREPORT_H
