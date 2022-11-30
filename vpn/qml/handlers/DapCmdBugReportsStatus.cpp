#include "DapCmdBugReportsStatus.h"
#include "DapBugReportHistory.h"

DapCmdBugReportsStatus::DapCmdBugReportsStatus (QObject *parent)
  : DapCmdClientAbstract (DapJsonCmdType::BUG_REPORTS_STATUS, parent)
{

}


void DapCmdBugReportsStatus::bugReportsStatusRequest (QList<DapBugReportHistoryItem> a_bugReportsList)
{
  if (a_bugReportsList.isEmpty())
    {
      qDebug() << "Empty bug-reports list";
      return;
    }

  QStringList list;
  for (const auto &item : a_bugReportsList)
    list << item.asString();

  QJsonObject obj
  {
    {"bug_reports_numbers_list", list.join (',')}
  };

  sendCmd (&obj);
}

void DapCmdBugReportsStatus::handleResult (const QJsonObject &result)
{
  QString request = result.value ("bug_reports_numbers_list_answer").toString();
  qDebug() << "Bug-reports status answer: " << request;
  emit sigBugReportStatusAnswer (request);
}

void DapCmdBugReportsStatus::handleError (int code, const QString &message)
{
  Q_UNUSED (code); Q_UNUSED (message);
  qWarning() << *m_errorObject;
}
