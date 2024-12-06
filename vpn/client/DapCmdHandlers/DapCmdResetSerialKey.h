#pragma once

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdResetSerialKey : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdResetSerialKey(QObject *parent = nullptr);
    ~DapCmdResetSerialKey() override {};

    void handle(const QJsonObject *params) override;

public slots:
    void sendResetSerialError(const int& a_errorCode, const QString&);

signals:
    void sigResetRequestSent();
    void sigResetSerialKeyReplied(const QString&);
};
