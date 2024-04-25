#ifndef DapCmdServersListSrv_H
#define DapCmdServersListSrv_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapServerInfo.h"

class DapCmdServersListSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdServersListSrv(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    ~DapCmdServersListSrv() override {}
    const QList<QString> &serversList() { return  m_serversList; }
signals:
    void nextCdb();
    void updateNodesList(const DapServerInfoList&);
    void sendCurrentServer();
public slots:
    void setServersList(const QList<QString>& a_serversList) { m_serversList = a_serversList ; }
private:
    QList<QString> m_serversList;
};

#endif // DapCmdServersListSrv_H
