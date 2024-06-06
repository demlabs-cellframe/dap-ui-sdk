#ifndef DapCmdTunTapSrvHANDLER_H
#define DapCmdTunTapSrvHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdTunTapSrv: public DapCmdServiceAbstract {
    Q_OBJECT
public:
    explicit DapCmdTunTapSrv(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::TAP_DRIVER, parent) {}
    virtual ~DapCmdTunTapSrv() override {}
    void handle(const QJsonObject* params) override;
    void sendCmd(const QString& val);
signals:
    void sigTapCheck();
    void sigTapInstall();
};

#endif // DapCmdTunTapSrvHANDLER_H
