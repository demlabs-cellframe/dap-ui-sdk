#ifndef DAPCONNECTIONDATA_H
#define DAPCONNECTIONDATA_H

#include <QObject>
#include <QDateTime>
#include <QtMath>
#include "DapSpeed.h"
#include "DapServerInfo.h"

class DapServerConnectionInfo : public QObject
{
    Q_OBJECT

public:
    explicit DapServerConnectionInfo(QObject *parent = nullptr);
    ~DapServerConnectionInfo();

    QDateTime startTime () const;
    quint64 bytesReceived   () const;
    quint64 bytesSent       () const;
    quint64 packetsReceived () const;
    quint64 packetsSent     () const;
    quint64 uploadSpeed     () const;
    quint64 downloadSpeed   () const;
    QString uploadSpeedString     () const;
    QString downloadSpeedString   () const;

    const DapServerInfo *serverInfo() const;
    void setServerInfo(const DapServerInfo &serverInfo);

public slots:
    void setStartTime(const QDateTime &a_startTime);

    void setStatistic(quint64 a_bytesReceived,
                      quint64 a_bytesSent,
                      quint64 a_packetsReceived,
                      quint64 a_packetsSent);

    void clearStatisticAndStartTime();

signals:

    void startTimeChanged(const QDateTime& a_time);
    void statisticSet(quint64 a_bytesReceived,
                      quint64 a_bytesSent,
                      quint64 a_packetsReceived,
                      quint64 a_packetsSent,
                      quint64 a_uploadSpeed,
                      quint64 a_downloadSpeed,
                      QString a_uploadSpeedString,
                      QString a_downloadSpeedString);

private:
    QDateTime   m_startTime {};

    quint64 m_bytesReceived   = 0;
    quint64 m_bytesSent       = 0;
    quint64 m_packetsReceived = 0;
    quint64 m_packetsSent     = 0;
    DapSpeed   m_uploadSpeed;
    DapSpeed   m_downloadSpeed;

    DapServerInfo m_serverInfo;

    QString convertBytePerSecond(const quint64 &bytes_new);
};

#endif // DAPCONNECTIONDATA_H
