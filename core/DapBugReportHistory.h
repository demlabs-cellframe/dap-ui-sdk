#ifndef DAPBUGREPORTHISTORY_H
#define DAPBUGREPORTHISTORY_H

#include <QObject>

struct _BugReportInfo
{
    QString number;
    QString status;

    _BugReportInfo(QString a_number, QString a_status){
        number = a_number;
        status = a_status;
    };

    _BugReportInfo& operator = (const _BugReportInfo &s)
    {
        status = s.status;
        number = s.number;
        return (*this);
    }
};

class DapBugReportHistory : public QObject
{
    Q_OBJECT
public:
    DapBugReportHistory(QObject *parent = nullptr);


    QList<_BugReportInfo> *getBugReportsList();

    void loadHistoryBugReportData();
    void setBugReportsList(QList<_BugReportInfo>*);
    QList<_BugReportInfo> bugReportsList(){
        return *m_bugReportsList;
    }
protected:
    void bugReportInfoListToStringList(QList<_BugReportInfo>);

    QList<_BugReportInfo> *m_bugReportsList;

signals:
    void bugReportListLoaded(QList<QString> a_bugReportsNumbersList);
};

#endif // DAPBUGREPORTHISTORY_H
