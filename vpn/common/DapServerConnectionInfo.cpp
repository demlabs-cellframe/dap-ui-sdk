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

int DapServerConnectionInfo::bytesReceived() const
{
    return m_bytesReceived;
}

int DapServerConnectionInfo::bytesSent() const
{
    return m_bytesSent;
}

int DapServerConnectionInfo::packetsReceived() const
{
    return m_packetsReceived;
}

int DapServerConnectionInfo::packetsSent() const
{
    return m_packetsSent;
}

void DapServerConnectionInfo::setStatistic(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent)
{
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;

    emit this->statisticSet(a_bytesReceived, a_bytesSent, a_packetsReceived, a_packetsSent);
}

void DapServerConnectionInfo::clearStatisticAndStartTime()
{
    this->setStartTime      ({});
    DapServerConnectionInfo::setStatistic(0, 0, 0, 0);
}
