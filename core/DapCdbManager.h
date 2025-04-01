#pragma once

#include <QString>
#include <QList>

class DapCdbServer {
public:
    QString address;
    int port;

    QString toString() const {
        return QString("%1:%2").arg(address).arg(port);
    }
};

class DapCdbManager {
public:
    static DapCdbManager& instance();

    void addServer(const QString& address, uint16_t port = 80);
    void clear();

    bool hasServers() const;
    DapCdbServer* currentServer();
    bool nextServer();
    void resetIndex();
    void setCurrentServer(int index);
    QStringList toStrings() const;
    const QList<DapCdbServer>& servers() const;

private:
    DapCdbManager() = default;
    DapCdbManager(const DapCdbManager&) = delete;
    DapCdbManager& operator=(const DapCdbManager&) = delete;

    QList<DapCdbServer> m_servers;
    int m_currentIndex = 0;
};
