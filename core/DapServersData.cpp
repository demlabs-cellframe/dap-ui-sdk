#include "DapBugReportData.h"

#include "DapDataLocal.h"

DapServersData::DapServersData()
{
    this->loadDatas();
}

DapServersData *DapServersData::instance()
{
    static DapServersData s_instance;
    return &s_instance;
}


void DapServersData::addServer(const DapServerInfo& dsi) {

    m_servers.push_back(dsi);

    emit this->serverAdded(dsi);
}

void DapServersData::addServer(DapServerLocation location, const QString& name,
                          const QString & address, quint16 port)
{
    DapServerInfo ss;
    ss.name = name;
    ss.location = location;
    ss.address = address;
    ss.port = port;
    addServer(std::move(ss));
}

void DapServersData::setCurrentServer(int a_serverIndex)
{
    if (m_currentServerIndex == a_serverIndex)
        return;
    Q_ASSERT(a_serverIndex < m_servers.count());
    m_currentServerIndex = a_serverIndex;

    emit currentServerNameChanged(this->currentServerName());
}

void DapServersData::setCurrentServer(const DapServerInfo *a_server)
{
    qDebug() << "DapDataLocal::setCurrentServer(" << (a_server ? a_server->name : "") << ")";

    setCurrentServer(m_servers.indexOf(*a_server));
}

/// Set server name.
/// @param server Server name.
void DapServersData::setCurrentServer(const QString &a_serverName)
{
    if (currentServerName() == a_serverName)
        return;

    for (int i = 0; i < this->serversCount(); i++)
    {
        if (m_servers.at(i).name == a_serverName)
        {
            setCurrentServer(i);
            return;
        }
    }

    qFatal("There is no server with name %s", qPrintable(a_serverName));
}

void DapServersData::saveDatas() const
{

}

void DapServersData::loadDatas()
{

}

const DapServerInfo *DapServersData::currentServer() const
{
    if (m_currentServerIndex < 0)
        return nullptr;

    return &m_servers.at(this->m_currentServerIndex);
}

int DapServersData::serversCount()
{
    m_servers.count();
}

void DapServersData::clearServerList()
{
    m_servers.clear();
    this->setCurrentServer(nullptr);

    emit this->serversCleared();
}

/// Get server name.
/// @return Server name.
QString DapServersData::currentServerName() const
{
    const DapServerInfo* currentServer = this->currentServer();
    return currentServer ? currentServer->name : "";
}

bool DapServersData::currentServerIsAuto() const
{
    return m_currentServerIsAuto;
}

void DapServersData::setCurrentServerIsAuto(bool a_auto)
{
    m_currentServerIsAuto = a_auto;
}
