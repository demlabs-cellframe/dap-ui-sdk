#ifndef DAPCMDSIGNUP_H
#define DAPCMDSIGNUP_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdSignUp : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdSignUp(QObject *parent = nullptr);
    ~DapCmdSignUp() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigSignUpRequest(const QString& site, const QString&, const QString&);
    void sigSendCmdSignUpAnswer(const QString& answer);
};

#endif // DAPCMDSIGNUP_H
