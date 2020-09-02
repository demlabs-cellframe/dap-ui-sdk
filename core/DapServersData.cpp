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
    int row = m_servers.count();
    insertRows(row, 1);
    setData(index(row, 0), qVariantFromValue(dsi));

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
        this->setCurrentServer(-1);
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
    if (m_currentServerIndex < 0 || m_currentServerIndex >= this->serversCount())
        return nullptr;

    return &m_servers.at(this->m_currentServerIndex);
}

int DapServersData::serversCount() const
{
    return m_servers.count();
}

void DapServersData::clearServerList()
{
    m_servers.clear();
    this->setCurrentServer(-1);

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
    return this->currentServer()->isAuto();
}

int DapServersData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_servers.count();
}

const QStringList DapServersData::m_countries = {
    "", //UNKNOWN = 0,
    "://country/Flag_uk.png", //ENGLAND,
    "://country/FR.png", //FRANCE,
    "://country/DE.png", //GERMANY,
    "://country/US.png", //USA,
    "://country/NL.png", //NETHERLANDS,
    "://country/RU.png", //RUSSIA,
    "://country/UA.png", //UKRAINE,
    "://country/NL.png", //Netherlands,
    "://country/SG.png", //Singapore,
    "://country/DE.png", //Germany,
};

QVariant DapServersData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_servers.size())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_servers.at(index.row()).name;
    case Qt::DecorationRole:
        return m_countries[static_cast<int>(m_servers.at(index.row()).location)];
    default:
        break;
    }

    return QVariant();
}

Qt::ItemFlags DapServersData::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

bool DapServersData::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_servers.replace(index.row(), value.value<DapServerInfo>());
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::DecorationRole});
        return true;
    }
    return false;
}

bool DapServersData::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        m_servers.insert(position, DapServerInfo());
    }
    endInsertRows();
    return true;
}

bool DapServersData::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        m_servers.removeAt(position);
    }

    endRemoveRows();
    return true;
}
