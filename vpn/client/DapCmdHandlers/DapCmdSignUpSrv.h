#ifndef DapCmdSignUpSrv_H
#define DapCmdSignUpSrv_H

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdSignUpSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdSignUpSrv(QObject *parent = nullptr);
    ~DapCmdSignUpSrv() override;

    void handle(const QJsonObject *params) override;

signals:
    void sigSignUpRequest(const QString& site, const QString&, const QString&);
    void sigSendCmdSignUpAnswer(const QString& answer);
};

#endif // DapCmdSignUpSrv_H
