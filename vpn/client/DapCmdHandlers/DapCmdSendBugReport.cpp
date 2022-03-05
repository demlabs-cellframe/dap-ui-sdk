#include "DapCmdSendBugReport.h"


DapCmdSendBugReport::DapCmdSendBugReport(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_BUG_REPORT, parent)
{
    connect(this, &DapCmdSendBugReport::sigSendCmdBugReportAnswer, [=](const QString& number){
        QJsonObject obj;
        obj["bugreport_answer"] = number;
        sendCmd(&obj);
    });
}

DapCmdSendBugReport::~DapCmdSendBugReport()
{

}

void DapCmdSendBugReport::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    QString serial = params->value("serial").toString();
    QString message = params->value("message").toString();
    qDebug() << QString("Received bug-report request");

    if (!serial.isNull() && !message.isNull()){

        emit sigBugReportSendRequest(serial, message);

    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
