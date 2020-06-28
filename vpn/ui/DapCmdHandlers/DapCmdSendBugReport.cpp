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
    QString request = result.value("bugreport_answer").toString();
    qDebug() << request;
    if (request.contains("saved successfully", Qt::CaseInsensitive))
        emit sigBugReportSent(request.remove(QRegExp("\\D")));
    else emit sigBugReportSendingError();
}

void DapCmdSendBugReport::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
