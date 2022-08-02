#ifndef DAPSERVERSDATA_H
#define DAPSERVERSDATA_H

#include <QObject>
#include <QAbstractListModel>

#include "DapServerInfo.h"
#include "DapLocationInfo.h"

#define COUTRY_FLAG_ROLE (Qt::UserRole + 10)
#define CONNECTION_QUALITY (Qt::UserRole + 11)
#define PING_ROLE (Qt::UserRole + 12)

class DapServersData: public QAbstractListModel
{
    Q_OBJECT
    DapServersData();  //private constructor becouse it's singletone
public:

    static DapServersData *instance();

    void addServer(const QString& location, const QString& name,
                   const QString & address, quint16 port);
    void addServer(const DapServerInfo& dsi);

    void packServerList();

    QList<DapServerInfo>& servers(){return m_servers;}
    QList<DapServerInfo>& bestRegionServers(){return m_bestRegionServerList;}
    QList<DapServerInfo>& pingServerList(){return m_pingServerList;}

    const DapServerInfo* currentServer() const;
    int serversCount() const;

    void clearServerList();

    QString currentServerName() const;
    QString currentServerAdress() const;

    bool currentServerIsAuto() const;

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    QMap<int, QVariant> itemData(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setBestPing(qint16 a_ping) {m_bestping = a_ping;}
    qint16 getBestPing () {return m_bestping;}
    void setWorstPing(qint16 a_ping) {m_worstping = a_ping;}
    qint16 getWorstPing () {return m_worstping;}

public slots:
    void setCurrentServer(const DapServerInfo *a_server);
    void setCurrentServer(const QString &a_serverName);
    void setCurrentServer(int a_serverIndex);
    void setCurrentServerNotSignal(int a_serverIndex);
    void setCurrentServerFromService(const DapServerInfo *a_server);

    void saveDatas() const;

signals:

    /// Signal emitted if server name has changed.
    /// @param serverName Server name.
    void currentServerNameChanged(const QString& serverName);
    void currentServerNameChangedNotRequestChangingServer(const QString& serverName);
    void serverAdded(const DapServerInfo& dsi);
    void serversCleared();

private:
    void loadDatas();
    static const QString findInCountriesMap(const QString& string);

    const QString CURRENT_SERVER_NAME_SETTING   = "email";
    const QString CURRENT_SERVER_ADDRESS_SETTING = "message";

 //   DapServerInfo* m_currentServer = nullptr;

    int m_currentServerIndex = -1;
    QList<DapServerInfo> m_servers;
    QList<DapServerInfo> m_bestRegionServerList;
    QList<DapServerInfo> m_pingServerList;
    DapLocationInfo m_locationInfo;
    static QMap<QString, QString> m_countryMap;

    qint16 m_bestping = 0;
    qint16 m_worstping = 0;
};

#endif // DAPSERVERSDATA_H
