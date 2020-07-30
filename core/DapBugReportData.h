#ifndef DAPBUGREPORTDATA_H
#define DAPBUGREPORTDATA_H

#include <QObject>

class DapBugReportData: public QObject
{
    Q_OBJECT
    DapBugReportData(); //private constructor becouse it's singletone
public:
    ~DapBugReportData();

    static DapBugReportData *instance();

    QString message() const;
    QString email() const;
    QString lastSentNumber() const;
    QString defaultEmail() const;

public slots:
    void setMessage(const QString &a_message);
    void setEmail(const QString &a_email);
    void setLastSentNumber(const QString& a_lastSentNumber = {});

    void saveDatas() const;

signals:
    void messageChanged(const QString& a_message);
    void emailChanged(const QString& a_email);
    void lastSentNumberChanged(const QString& a_lastSent);

private:
    void loadDatas();

    QString m_email;
    QString m_message;
    QString m_lastSentNumber {}; //QString() - sending error

    const QString SETTINGS_TEXT_EMAIL   = "email";
    const QString SETTINGS_TEXT_MESSAGE = "message";
    const QString SETTINGS_TEXT_LAST_SENT_NUMBER = "lastSentNumber";
};

#endif // DAPBUGREPORTDATA_H
