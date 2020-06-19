#ifndef DAPCMDSENDBUGREPORT_H
#define DAPCMDSENDBUGREPORT_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdClientAbstract.h"


class DapCmdSendBugReport: public DapCmdClientAbstract
{
    Q_OBJECT
    QString email;
    QString message;

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

public:

    explicit DapCmdSendBugReport(QObject *parent = nullptr);
    virtual ~DapCmdSendBugReport() override { }

    void sendingBugReportRequest(const QString &address, const QString &message);
signals:
    void sigBugReportSent(const QString& bugReportNumber);
};


#endif // DAPCMDSENDBUGREPORT_H
