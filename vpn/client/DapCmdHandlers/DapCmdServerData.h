#pragma once

#include <QObject>
#include "DapCmdServiceAbstract.h"

class DapCmdServerData: public DapCmdServiceAbstract {
    Q_OBJECT
public:
    explicit DapCmdServerData(QObject *parent = nullptr) : DapCmdServiceAbstract(DapJsonCmdType::SEND_SERVER_DATA, parent) {

    }
    virtual ~DapCmdServerData() override {

    }
    void handle(const QJsonObject* params) override;
signals:
    void sigChangeServerData(const QString& address, uint16_t port);
};
