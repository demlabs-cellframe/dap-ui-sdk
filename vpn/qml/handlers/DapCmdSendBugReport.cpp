#include "DapCmdSendBugReport.h"

DapCmdSendBugReport::DapCmdSendBugReport(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_BUG_REPORT, parent)
{

}

void DapCmdSendBugReport::sendBugReport(
    const QString &a_message,
    const QString &a_serial,
    const QString &attachFile,
    const QString &a_emailAddress)
{
    QJsonObject obj =
    {
      {"attach_file", attachFile},
      {"email", a_emailAddress},
      {"message", a_message},
      {"serial", a_serial},
    };
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
