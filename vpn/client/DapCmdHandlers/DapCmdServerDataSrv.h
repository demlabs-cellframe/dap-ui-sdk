#pragma once

#include <QObject>
#include "DapCmdServiceAbstract.h"

#include "DapServerInfo.h"

class DapCmdServerDataSrv: public DapCmdServiceAbstract {
    Q_OBJECT
public:
    explicit DapCmdServerDataSrv(QObject *parent = nullptr) : DapCmdServiceAbstract(DapJsonCmdType::SEND_SERVER_DATA, parent) {

    }
    virtual ~DapCmdServerDataSrv() override {

    }
    void handle(const QJsonObject* params) override;

    void sendServerInfo(const DapServerInfo &server);
signals:
    void sigChangeServerData(const QString& address, uint16_t port);
};
