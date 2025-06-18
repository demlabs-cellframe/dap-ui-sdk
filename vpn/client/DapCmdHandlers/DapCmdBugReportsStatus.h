#ifndef DAPCMDBUGREPORTSSTATUS_H
#define DAPCMDBUGREPORTSSTATUS_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdBugReportsStatus : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdBugReportsStatus(QObject *parent = nullptr);
    ~DapCmdBugReportsStatus() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigBugReportsStatusSendRequest(const QString&);
    void sigSendCmdBugReportStatusAnswer(const QString& answer);
};

#endif // DAPCMDBUGREPORTSSTATUS_H
