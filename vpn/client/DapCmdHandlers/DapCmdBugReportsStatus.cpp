#include "DapCmdBugReportsStatus.h"

DapCmdBugReportsStatus::DapCmdBugReportsStatus(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::BUG_REPORTS_STATUS, parent)
{
    connect(this, &DapCmdBugReportsStatus::sigSendCmdBugReportStatusAnswer, [=](const QString& answer){
        QJsonObject obj;
        obj["bug_reports_numbers_list_answer"] = answer;
        sendCmd(&obj);
    });
}

DapCmdBugReportsStatus::~DapCmdBugReportsStatus()
{

}

void DapCmdBugReportsStatus::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);

    QString request = params->value("bug_reports_numbers_list").toString();
    qDebug() << QString("Received bug-report status request");

    if (!request.isNull()){
        emit sigBugReportsStatusSendRequest(request);
    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
