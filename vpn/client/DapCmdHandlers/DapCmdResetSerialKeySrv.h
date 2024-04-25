#pragma once

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdResetSerialKeySrv : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdResetSerialKeySrv(QObject *parent = nullptr);
    ~DapCmdResetSerialKeySrv() override {};

    void handle(const QJsonObject *params) override;

public slots:
    void sendResetSerialError(const int& a_errorCode, const QString&);

signals:
    void sigResetRequestSent(const QString&);
    void sigResetSerialKeyReplied(const QString&);
};
