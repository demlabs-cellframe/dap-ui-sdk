#ifndef DAPSERVERSTORAGE_H
#define DAPSERVERSTORAGE_H

#include <QObject>
#include <QVector>
#include <DapServerInfo.h>
#include <DapLocationInfo.h>
#include <QFile>


class DapServersLocalStorage : public QObject
{
    Q_OBJECT
private:
    QString _serversListFilePath; //
    QJsonArray _jsonServersListArr; // mirrors array from QSettings
    // it's a _jsonServersListArr in the native view
    DapServerInfoList _serversList;
    DapLocationInfo _locationInfo;

    int _rewriteJsonFile();
    int _loadServers(); // fill _serversList from QSettings
protected:
    bool isServerExists(const DapServerInfo& server);
public:
    explicit DapServersLocalStorage(QObject *parent = nullptr,
                                    const QString& fileName = "serversList.json");

    int addServer(const DapServerInfo& server);
    int removeServer(const QString& address, const quint16 port);
    int removeServer(const DapServerInfo& server);

    int editServer(const DapServerInfo& oldServer, const DapServerInfo& newServer);

    const DapServerInfoList& getServersList() { return _serversList; }
signals:
    void changed();
public slots:
};

#endif // DAPSERVERSTORAGE_H
