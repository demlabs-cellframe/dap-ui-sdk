#include "DapCmdBugReportsStatus.h"

DapCmdBugReportsStatus::DapCmdBugReportsStatus(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::BUG_REPORTS_STATUS, parent)
{

}

DapCmdBugReportsStatus::~DapCmdBugReportsStatus()
{

}

void DapCmdBugReportsStatus::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    connect(this, &DapCmdBugReportsStatus::sigSendCmdBugReportAnswer, [=](const QString& answer){
        QJsonObject obj;
        obj["bug_reports_numbers_list_answer"] = answer;
        sendCmd(&obj);
    });

    QString request = params->value("bug_reports_numbers_list").toString();
    qDebug() << QString("Received bug-report request");

    if (!request.isNull()){
        emit sigBugReportsStatusSendRequest(request);
    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
