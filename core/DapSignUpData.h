#ifndef DAPSIGNUPDATA_H
#define DAPSIGNUPDATA_H

#include <QObject>

class DapSignUpData: public QObject
{
    Q_OBJECT
public:
    DapSignUpData();

    static DapSignUpData *instance();

    QString email() const;
    QString password() const;
    QString dataAnswer() const;

public slots:
    void setEmail(const QString &a_email);
    void setPassword(const QString &a_password);
    void setDataAnswer(const QString &a_data);

    void saveDatas() const;

signals:
    void passwordChanged(const QString& a_password);
    void emailChanged(const QString& a_email);

private:
    void loadDatas();

    QString m_email;
    QString m_password;
    QString m_data;

    const QString SETTINGS_TEXT_EMAIL   = "email";
    const QString SETTINGS_TEXT_PASSWORD = "password";

};

#endif // DAPSIGNUPDATA_H
