#include "DapAlternativeServersContainer.h"
#include <QRandomGenerator>

DapAlternativeServersContainer::DapAlternativeServersContainer(QObject *a_parent /*= nullptr*/)
    : QObject(a_parent)
{
}

DapServerInfo DapAlternativeServersContainer::takeNext(const QString& a_location)
{
    QList<DapServerInfo> serversList;
    if (a_location == "UNKNOWN")
    {
        if (m_servers.isEmpty())
            return {};

        serversList = m_servers.values();
    }
    else {
        if (!m_servers.contains(a_location))
            return {};

        serversList = m_servers.values(a_location);
    }

    int randomIndex = QRandomGenerator::global()->bounded(serversList.size());

    DapServerInfo randomServer = serversList.value(randomIndex);
    m_servers.remove(randomServer.location(), randomServer);
    return randomServer;
}

void DapAlternativeServersContainer::refill(const QList<DapServerInfo> &a_serversList)
{
    for(const DapServerInfo& currentServer: a_serversList)
    {
        if (currentServer.isAuto() || this->hasAddress(currentServer.address()))
            continue;
        m_servers.insertMulti(currentServer.location(), currentServer);
    }
}

bool DapAlternativeServersContainer::hasAddress(const QString &a_address)
{
    for (const DapServerInfo& currentServer: m_servers)
    {
        if (currentServer.address() == a_address)
            return true;
    }
    return false;
}
