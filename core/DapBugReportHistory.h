#ifndef DAPBUGREPORTHISTORY_H
#define DAPBUGREPORTHISTORY_H

#include <QObject>

struct _BugReportInfo
{
    QString number;
    QString status;
};

class DapBugReportHistory : public QObject
{
    Q_OBJECT
public:
    DapBugReportHistory(QObject *parent = nullptr);


    QList<_BugReportInfo> getBugReportsList();

    void getHistoryBugReportData();
protected:
    void bugReportInfoListToStringList(QList<_BugReportInfo>);

    QList<_BugReportInfo> *m_bugReportsList;

signals:
    void bugReportListLoaded(QList<QString> a_bugReportsNumbersList);
};

#endif // DAPBUGREPORTHISTORY_H
