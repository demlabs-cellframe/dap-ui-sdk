#ifndef DapCmdSendBugReportSrv_H
#define DapCmdSendBugReportSrv_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdSendBugReportSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdSendBugReportSrv(QObject *parent = nullptr);
    ~DapCmdSendBugReportSrv() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigBugReportSendRequest(const QString&, const QString&, const QString&, const QString&);
    void sigSendCmdBugReportAnswer(const QString& number);
};

#endif // DapCmdSendBugReportSrv_H
