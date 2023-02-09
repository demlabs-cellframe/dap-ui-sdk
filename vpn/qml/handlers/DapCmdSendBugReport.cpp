#include "DapCmdSendBugReport.h"

DapCmdSendBugReport::DapCmdSendBugReport (QObject *a_parent)
  : DapCmdClientAbstract (DapJsonCmdType::SEND_BUG_REPORT, a_parent)
  , m_waitingForResponse (false)
{

}

void DapCmdSendBugReport::sendBugReport(
    const QString &a_message,
    const QString &a_serial,
    const QString &attachFile,
    const QString &a_emailAddress)
{
  /* set flag */
  m_waitingForResponse  = true;

  /* send report */
  QJsonObject obj =
  {
    {"attach_file", attachFile},
    {"email", a_emailAddress},
    {"message", a_message},
    {"serial", a_serial},
  };
  sendCmd (&obj);
}

void DapCmdSendBugReport::cancelBugReport()
{
  m_waitingForResponse  = false;
}

void DapCmdSendBugReport::handleResult (const QJsonObject& a_result)
{
  if (!m_waitingForResponse)
    return;

  QString request = a_result.value ("bugreport_answer").toString();

  qDebug() << "Bug report answer: " << request;

  if (request.contains ("saved successfully", Qt::CaseInsensitive))
    emit sigBugReportSent (request.remove (QRegExp("\\D")));
  else
    emit sigBugReportSent ("");

  m_waitingForResponse = false;
}

void DapCmdSendBugReport::handleError (int, const QString&)
{
  qWarning() << *m_errorObject;
}
