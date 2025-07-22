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

    void handleReplyFinished(DapNetworkReply* reply);
    void handleReplyError(DapNetworkReply* reply);
    void processNextCDB(int errorCode, const QString& errorMessage);
    void sendRequestToCurrentCDB(DapNetworkReply* reply);

    void saveServerListToSettings(const QJsonArray& arr, const QString& time);
    void emitServerListUpdate(const QJsonArray& arr, const QString& time);

signals:
    void nextCdb();
    void updateNodesList(const DapServerInfoList&, QString a_time);
    void sendCurrentServer();

public slots:
    void setServersList(const QList<QString>& a_serversList) { m_serversList = a_serversList ; }
    void sendServerList(DapServerInfoList m_nodelist, QString time);
    void setNoCdbMode(bool enabled) { m_noCdbMode = enabled; }

private:
    QList<QString> m_serversList;
    bool guiCall;
    bool m_noCdbMode = false;
    QTimer * emitTimer;

    QJsonArray filterUnavailableServers(const QJsonArray& arr);
    void handleCdbIteratorError();
    void sendRequestToCDB();
};

#endif // DAPCMDSERVERSLIST_H
