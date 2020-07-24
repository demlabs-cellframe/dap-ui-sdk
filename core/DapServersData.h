#ifndef DAPSERVERSDATA_H
#define DAPSERVERSDATA_H

#include <QObject>

#include "DapServerInfo.h"

class DapServersData: public QObject
{
    Q_OBJECT
    DapServersData();  //private constructor becouse it's singletone
public:

    static DapServersData *instance();

    void addServer(DapServerLocation location, const QString& name,
                   const QString & address, quint16 port);
    void addServer(const DapServerInfo& dsi);

    QList<DapServerInfo>& servers(){return m_servers;}

    const DapServerInfo* currentServer() const;
    int serversCount() const;

    void clearServerList();

    QString currentServerName() const;

    bool currentServerIsAuto() const;
    void setCurrentServerIsAuto(bool a_auto = true);

public slots:
    void setCurrentServer(const DapServerInfo *a_server);
    void setCurrentServer(const QString &a_serverName);
    void setCurrentServer(int a_serverIndex);

    void saveDatas() const;

signals:

    /// Signal emitted if server name has changed.
    /// @param serverName Server name.
    void currentServerNameChanged(const QString& serverName);
    void serverAdded(const DapServerInfo& dsi);
    void serversCleared();

private:
    void loadDatas();


    const QString CURRENT_SERVER_NAME_SETTING   = "email";
    const QString CURRENT_SERVER_ADDRESS_SETTING = "message";

 //   DapServerInfo* m_currentServer = nullptr;

    int m_currentServerIndex;
    bool m_currentServerIsAuto;
    QList<DapServerInfo> m_servers;
};

#endif // DAPSERVERSDATA_H
