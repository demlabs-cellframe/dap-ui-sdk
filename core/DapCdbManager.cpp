#include "DapCdbManager.h"
#include <QDebug>

DapCdbManager& DapCdbManager::instance() {
    static DapCdbManager instance;
    return instance;
}

void DapCdbManager::addServer(const QString& address, uint16_t port) {
    DapCdbServer server;
    server.address = address;
    server.port = port;
    m_servers.push_back(server);
    qInfo() << "[DapCdbManager] Added CDB server:" << address << ":" << port;
}

void DapCdbManager::clear() {
    m_servers.clear();
    m_currentIndex = 0;
}

bool DapCdbManager::hasServers() const {
    return !m_servers.isEmpty();
}

DapCdbServer* DapCdbManager::currentServer() {
    if (m_servers.isEmpty()) return nullptr;
    if (m_currentIndex < 0 || m_currentIndex >= m_servers.size()) return nullptr;
    return &m_servers[m_currentIndex];
}

bool DapCdbManager::nextServer() {
    if (m_servers.isEmpty()) return false;
    if (m_currentIndex + 1 < m_servers.size()) {
        ++m_currentIndex;
        return true;
    }
    return false;
}

void DapCdbManager::resetIndex() {
    m_currentIndex = 0;
}

void DapCdbManager::setCurrentServer(int index) {
    if (index >= 0 && index < m_servers.size()) {
        m_currentIndex = index;
    } else {
        qWarning() << "[DapCdbManager] Invalid index setCurrentServer(" << index << ")";
    }
}

QStringList DapCdbManager::toStrings() const {
    QStringList result;
    for (const auto& server : m_servers) {
        result << QString("%1:%2").arg(server.address).arg(server.port);
    }
    return result;
}

const QList<DapCdbServer>& DapCdbManager::servers() const {
    return m_servers;
}


