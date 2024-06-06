#ifndef DapCmdNewsSrv_H
#define DapCmdNewsSrv_H

#include "DapCmdServiceAbstract.h"

class DapCmdNewsSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
public:

    DapCmdNewsSrv(QObject *parent = nullptr);
    void handle(const QJsonObject *params) override;

signals:
    void sigReceivedNewsMessage(const QJsonDocument& news);
    void getNews();
};

#endif // DapCmdNewsSrv_H
