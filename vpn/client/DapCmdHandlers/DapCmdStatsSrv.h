#ifndef DapCmdStatsSrv_H
#define DapCmdStatsSrv_H

#include "DapCmdServiceAbstract.h"

class DapCmdStatsSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdStatsSrv(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;

    void sendCmdStats(qint64 readBytes, qint64 writeBytes,
                      qint64 readPackages, qint64 writePackages);
signals:
    void sigStreamOn();
    void sigStreamOff();
};

#endif // DapCmdStatsSrv_H
