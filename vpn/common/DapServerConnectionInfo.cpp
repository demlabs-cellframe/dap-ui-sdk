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

QString DapServerConnectionInfo::uploadSpeed() const
{
    return m_uploadSpeed;
}

QString DapServerConnectionInfo::downloadSpeed() const
{
    return m_downloadSpeed;
}

void DapServerConnectionInfo::setStatistic(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent)
{
    m_uploadSpeed     = convertBytePerSecond(a_bytesSent - m_bytesSent);
    m_downloadSpeed   = convertBytePerSecond(a_bytesReceived - m_bytesReceived);
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;

    emit this->statisticSet(a_bytesReceived, a_bytesSent, a_packetsReceived, a_packetsSent, m_uploadSpeed, m_downloadSpeed);
}

QString DapServerConnectionInfo::convertBytePerSecond(const quint64 &byte)
{
    if (byte < 0)
        return QString("0 %1").arg("Mbps");
    else if (byte >= pow(2,20))
        return QString("%1 %2").arg(QString::number(byte/pow(2,20), 'f', 2)).arg("Mbps");
    else
        return QString("%1 %2").arg(QString::number(byte/pow(2,10), 'f', 2)).arg("Kbps");
}

void DapServerConnectionInfo::clearStatisticAndStartTime()
{
    this->setStartTime      ({});
    DapServerConnectionInfo::setStatistic(0, 0, 0, 0);
}

const DapServerInfo* DapServerConnectionInfo::serverInfo() const
{
    return &m_serverInfo;
}

void DapServerConnectionInfo::setServerInfo(const DapServerInfo &serverInfo)
{
    m_serverInfo = serverInfo;
}
