#ifndef DAPCMDBUGREPORTSSTATUS_H
#define DAPCMDBUGREPORTSSTATUS_H

#include <QObject>
#include "DapCmdClientAbstract.h"

struct DapBugReportHistoryItem;

class DapCmdBugReportsStatus : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdBugReportsStatus(QObject *parent = nullptr);
    virtual ~DapCmdBugReportsStatus() override { }

    void bugReportsStatusRequest (QList<DapBugReportHistoryItem> a_bugReportsList);

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

signals:
    void sigBugReportStatusAnswer(const QString &answer);

public slots:
};

#endif // DAPCMDBUGREPORTSSTATUS_H
