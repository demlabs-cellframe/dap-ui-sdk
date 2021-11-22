#pragma once

#include "DapCmdServiceAbstract.h"
#include <QJsonObject>

class DapCmdResetSerialKey : public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdResetSerialKey(QObject *parent = nullptr)
        : DapCmdServiceAbstract (DapJsonCmdType::SEND_BUG_REPORT, parent) {};
    ~DapCmdResetSerialKey() override {};

    void handle(const QJsonObject *params) override;
signals:
    void sigResetRequestSent(const QString&);
    void sigResetRequestReplied(const QString&);
};
