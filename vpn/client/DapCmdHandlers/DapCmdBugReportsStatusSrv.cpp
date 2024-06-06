#include "DapCmdBugReportsStatusSrv.h"

DapCmdBugReportsStatusSrv::DapCmdBugReportsStatusSrv(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::BUG_REPORTS_STATUS, parent)
{
    connect(this, &DapCmdBugReportsStatusSrv::sigSendCmdBugReportStatusAnswer, [=](const QString& answer){
        QJsonObject obj;
        obj["bug_reports_numbers_list_answer"] = answer;
        sendCmd(&obj);
    });
}

DapCmdBugReportsStatusSrv::~DapCmdBugReportsStatusSrv()
{

}

void DapCmdBugReportsStatusSrv::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    QString request = params->value("bug_reports_numbers_list").toString();
    qDebug() << QString("Received bug-report status request");

    if (!request.isNull()){
        emit sigBugReportsStatusSendRequest(request);
    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
