#ifndef DAPCMDSERVERSLIST_H
#define DAPCMDSERVERSLIST_H

#include <QObject>
#include <QTimer>
#include "DapCmdServiceAbstract.h"
#include "DapServerInfo.h"
#include "DapNetworkReply.h"

class DapCmdServersList: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdServersList(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    ~DapCmdServersList() override {}
    const QList<QString> &serversList() { return  m_serversList; }
    void updateServerList(const QJsonArray& arr);
    bool loadServerList();
signals:
    void nextCdb();
    void updateNodesList(const DapServerInfoList&);
    void sendCurrentServer();
public slots:
    void setServersList(const QList<QString>& a_serversList) { m_serversList = a_serversList ; }
private:
    QList<QString> m_serversList;
    bool guiCall;
    QTimer * emitTimer;
    bool m_errorSentOnce = false;

    void sendServerList(const QJsonArray& arr, const QString &time);
    void sendRequestToCDB();

};

#endif // DAPCMDSERVERSLIST_H
