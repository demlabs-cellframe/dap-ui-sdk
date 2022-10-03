#ifndef DapCmdSignUp_H
#define DapCmdSignUp_H

#include "DapCmdClientAbstract.h"

class DapCmdSignUp: public DapCmdClientAbstract
{
    Q_OBJECT
    QString email;
    QString message;

protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;

public:

    explicit DapCmdSignUp(QObject *parent = nullptr);
    virtual ~DapCmdSignUp() override { }

    void sendSignUpRequest(const QString &a_site, const QString &a_email, const QString &a_password);
signals:
    void sigSignUpAnswer(const QString &a_dataAnswerSignUp);
    void sigBugReportSendingError();
};

#endif // DapCmdSignUp_H
