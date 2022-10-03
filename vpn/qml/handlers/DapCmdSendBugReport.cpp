#include "DapCmdSendBugReport.h"

DapCmdSendBugReport::DapCmdSendBugReport(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_BUG_REPORT, parent)
{

}

void DapCmdSendBugReport::sendBugReport(const QString &a_message, const QString &a_serial /*= ""*/, const QString &attachFile)
{
    QJsonObject obj;
    obj["serial"] = a_serial;
    obj["message"] = a_message;
    obj["attach_file"] = attachFile;
    sendCmd(&obj);
}

void DapCmdSendBugReport::handleResult(const QJsonObject& result)
{
    QString request = result.value("bugreport_answer").toString();
    qDebug() << "Bug report answer: " << request;
    if (request.contains("saved successfully", Qt::CaseInsensitive))
        emit sigBugReportSent(request.remove(QRegExp("\\D")));
    else emit sigBugReportSent("");
}

void DapCmdSendBugReport::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
