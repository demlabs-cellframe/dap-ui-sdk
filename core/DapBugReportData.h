#ifndef DAPBUGREPORTDATA_H
#define DAPBUGREPORTDATA_H

#include <QObject>

class DapBugReportData: public QObject
{
    Q_OBJECT
    DapBugReportData(); //private constructor becouse it's singletone
public:

    static DapBugReportData *instance();

    QString message() const;
    QString email() const;

public slots:
    void setMessage(const QString &a_message);
    void setEmail(const QString &a_email);

    void saveDatas() const;

signals:
    void messageChanged(const QString& a_message);
    void emailChanged(const QString& a_email);

private:
    void loadDatas();

    QString m_email;
    QString m_message;

    const QString SETTINGS_TEXT_EMAIL   = "email";
    const QString SETTINGS_TEXT_MESSAGE = "message";
};

#endif // DAPBUGREPORTDATA_H
