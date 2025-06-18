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
    DapCmdServiceAbstract::handle(params);

    QString serial = params->value("serial").toString();
    QString message = params->value("message").toString();
    QString contactAddress = params->value("contact_address").toString();
    QString attachFile = params->value("attach_file").toString();
    qDebug() << QString("Received bug-report request");

    if (!serial.isNull() && !message.isNull() && !attachFile.isNull()){

        emit sigBugReportSendRequest(serial, message, contactAddress, attachFile);

    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
