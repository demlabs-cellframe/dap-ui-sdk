#ifndef DAPCMDSTATS_H
#define DAPCMDSTATS_H

#include "DapCmdServiceAbstract.h"

class DapCmdStats: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdStats(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;

    void sendCmdStats(qint64 readBytes, qint64 writeBytes,
                      qint64 readPackages, qint64 writePackages);
signals:
    void sigStreamOn();
    void sigStreamOff();
};

#endif // DAPCMDSTATS_H
