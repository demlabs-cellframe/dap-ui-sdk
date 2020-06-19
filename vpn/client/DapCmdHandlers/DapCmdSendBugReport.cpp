#include "DapCmdSendBugReport.h"


DapCmdSendBugReport::DapCmdSendBugReport(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_BUG_REPORT, parent)
{

}

DapCmdSendBugReport::~DapCmdSendBugReport()
{

}

void DapCmdSendBugReport::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    QString email = params->value("email").toString();
    QString message = params->value("message").toString();

    if (!email.isNull() && !message.isNull()){

        emit sigBugReportSendRequest(email, message);

    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
