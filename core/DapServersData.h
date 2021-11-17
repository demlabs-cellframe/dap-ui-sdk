#ifndef DAPSERVERSDATA_H
#define DAPSERVERSDATA_H

#include <QObject>
#include <QAbstractListModel>

#include "DapServerInfo.h"

#define COUTRY_FLAG_ROLE (Qt::UserRole + 10)

class DapServersData: public QAbstractListModel
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

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
    QMap<int, QVariant> itemData(const QModelIndex &index) const Q_DECL_OVERRIDE;

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
    static const QStringList m_countries;
    static QMap<QString, QString> m_countryMap;
};

#endif // DAPSERVERSDATA_H
