#include "DapCmdBugReportsStatus.h"

DapCmdBugReportsStatus::DapCmdBugReportsStatus(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::BUG_REPORTS_STATUS, parent)
{

}


void DapCmdBugReportsStatus::bugReportsStatusRequest(QList<QString> a_bugReportsNumbersList)
{
    QJsonObject obj;
    QString temp_string;

    for (auto item : a_bugReportsNumbersList){
        temp_string += item + ",";
    }
    temp_string.chop(1);

    obj["bug_reports_numbers_list"] = temp_string;
    sendCmd(&obj);
}

void DapCmdBugReportsStatus::handleResult(const QJsonObject& result)
{
    QString request = result.value("bug_reports_numbers_list_answer").toString();
    qDebug() << "Bug-reports status answer: " << request;
    emit sigBugReportStatusAnswer(request);
}

void DapCmdBugReportsStatus::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
