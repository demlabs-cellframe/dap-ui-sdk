#include "DapServerConnectionInfo.h"

DapServerConnectionInfo::DapServerConnectionInfo(QObject *parent) : QObject(parent)
{
}

DapServerConnectionInfo::~DapServerConnectionInfo()
{
}

QDateTime DapServerConnectionInfo::startTime() const
{
    return m_startTime;
}

void DapServerConnectionInfo::setStartTime(const QDateTime &a_startTime)
{
    if (m_startTime == a_startTime)
        return;

    m_startTime = a_startTime;
    emit this->startTimeChanged(a_startTime);
}

quint64 DapServerConnectionInfo::bytesReceived() const
{
    return m_bytesReceived;
}

quint64 DapServerConnectionInfo::bytesSent() const
{
    return m_bytesSent;
}

quint64 DapServerConnectionInfo::packetsReceived() const
{
    return m_packetsReceived;
}

quint64 DapServerConnectionInfo::packetsSent() const
{
    return m_packetsSent;
}

quint64 DapServerConnectionInfo::uploadSpeed() const
{
    return m_uploadSpeed.speed();
}

quint64 DapServerConnectionInfo::downloadSpeed() const
{
    return m_downloadSpeed.speed();
}

QString DapServerConnectionInfo::uploadSpeedString() const
{
    return m_uploadSpeed.asString();
}

QString DapServerConnectionInfo::downloadSpeedString() const
{
    return m_downloadSpeed.asString();
}

void DapServerConnectionInfo::setStatistic(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent)
{
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;
    m_uploadSpeed.setTraffic(m_bytesSent);
    m_downloadSpeed.setTraffic(m_bytesReceived);
    emit this->statisticSet(a_bytesReceived, a_bytesSent, a_packetsReceived, a_packetsSent,
                            m_uploadSpeed.speed(), m_downloadSpeed.speed(), m_uploadSpeed.asString(), m_downloadSpeed.asString());
}

void DapServerConnectionInfo::clearStatisticAndStartTime()
{
    this->setStartTime      ({});
    DapServerConnectionInfo::setStatistic(0, 0, 0, 0);
    m_uploadSpeed.reset();
    m_downloadSpeed.reset();
    emit this->statisticSet(m_bytesReceived, m_bytesSent, m_packetsReceived, m_packetsSent,
                            m_uploadSpeed.speed(), m_downloadSpeed.speed(), m_uploadSpeed.asString(), m_downloadSpeed.asString());
}

const DapServerInfo* DapServerConnectionInfo::serverInfo() const
{
    return &m_serverInfo;
}

void DapServerConnectionInfo::setServerInfo(const DapServerInfo &serverInfo)
{
    m_serverInfo = serverInfo;
}

//void DapServerConnectionInfo::setState(ConnectionState a_state)
//{
//    if (a_state == m_state)
//        return;
//    m_state = a_state;
//    if (m_state == ConnectionState::Disconnected)
//    {
//        clearStatisticAndStartTime();
//    }
//}
