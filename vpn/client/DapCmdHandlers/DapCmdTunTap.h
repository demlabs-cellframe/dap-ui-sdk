#ifndef DAPCMDTUNTAPHANDLER_H
#define DAPCMDTUNTAPHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdTunTap: public DapCmdServiceAbstract {
    Q_OBJECT
public:
    explicit DapCmdTunTap(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::TAP_DRIVER, parent) {}
    virtual ~DapCmdTunTap() override {}
    void handle(const QJsonObject* params) override;
    void sendCmd(const QString& val);
signals:
    void sigTapCheck();
    void sigTapInstall();
};

#endif // DAPCMDTUNTAPHANDLER_H
