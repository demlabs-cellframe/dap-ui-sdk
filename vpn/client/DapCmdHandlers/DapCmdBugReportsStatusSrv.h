#ifndef DapCmdBugReportsStatusSrv_H
#define DapCmdBugReportsStatusSrv_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdBugReportsStatusSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdBugReportsStatusSrv(QObject *parent = nullptr);
    ~DapCmdBugReportsStatusSrv() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigBugReportsStatusSendRequest(const QString&);
    void sigSendCmdBugReportStatusAnswer(const QString& answer);
};

#endif // DapCmdBugReportsStatusSrv_H
