#ifndef DAPCMDSENDBUGREPORT_H
#define DAPCMDSENDBUGREPORT_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdSendBugReport : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdSendBugReport(QObject *parent = nullptr);
    ~DapCmdSendBugReport() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigBugReportSendRequest(const QString&, const QString&, const QString&, const QString&);
    void sigSendCmdBugReportAnswer(const QString& number);
};

#endif // DAPCMDSENDBUGREPORT_H
