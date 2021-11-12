#ifndef DAPCMDSERVERSLIST_H
#define DAPCMDSERVERSLIST_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapServerInfo.h"

class DapCmdServersList: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdServersList(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    ~DapCmdServersList() override {}
    const QList<QString> &serversList() { return  m_serversList; }
signals:
    void nextCdb();
    void updateNodesList(const DapServerInfoList&);
    void sendCurrent();
public slots:
    void setServersList(const QList<QString>& a_serversList) { m_serversList = a_serversList ; }
private:
    QList<QString> m_serversList;
};

#endif // DAPCMDSERVERSLIST_H
