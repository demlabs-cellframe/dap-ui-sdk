#include "DapCmdSendBugReport.h"
#include <QTimer>

DapCmdSendBugReport::DapCmdSendBugReport (QObject *a_parent)
  : DapCmdClientAbstract (DapJsonCmdType::SEND_BUG_REPORT, a_parent)
  , m_waitingForResponse (false)
{

}

void DapCmdSendBugReport::sendBugReport(
    const QString &a_message,
    const QString &a_serial,
    const QString &attachFile,
    const QString &a_contactAddress)
{
  if (!m_waitingForResponse)
  {
    /* set flag */
    m_waitingForResponse  = true;
    return;
  }

  /* send report */
  QJsonObject obj =
  {
    {"attach_file", attachFile},
    {"contact_address", a_contactAddress},
    {"message", a_message},
    {"serial", a_serial},
  };
  sendCmd (&obj);
}

void DapCmdSendBugReport::cancelBugReport()
{
  m_waitingForResponse  = false;
  QTimer::singleShot (2000, this, [this] { m_waitingForResponse  = true; });
}

void DapCmdSendBugReport::handleResult (const QJsonObject& a_result)
{
  if (!m_waitingForResponse)
    return;

  QString request = a_result.value ("bugreport_answer").toString();

  qDebug() << "Bug report answer: " << request;

  if (request.contains ("saved successfully", Qt::CaseInsensitive))
    emit sigBugReportSent (request.remove (QRegularExpression("\\D")));
  else
    emit sigBugReportSent ("");

  m_waitingForResponse = false;
}

void DapCmdSendBugReport::handleError (int, const QString&)
{
  qWarning() << *m_errorObject;
}
