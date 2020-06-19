#include "DapCmdSendBugReport.h"

DapCmdSendBugReport::DapCmdSendBugReport(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_BUG_REPORT, parent)
{

}

void DapCmdSendBugReport::sendingBugReportRequest(const QString &email, const QString &message)
{
    QJsonObject obj;
    obj["email"] = email;
    obj["message"] = message;
    sendCmd(&obj);
}
void DapCmdSendBugReport::handleResult(const QJsonObject& result)
{
    QString bugReportNumber = result.value("bugreport_number").toString();
    qDebug() << "Bug report number:" << bugReportNumber;
    emit sigBugReportSent(bugReportNumber);
}

void DapCmdSendBugReport::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
