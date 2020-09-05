#ifndef DAPCONNECTIONDATA_H
#define DAPCONNECTIONDATA_H

#include <QObject>
#include <QDateTime>
#include <QtMath>

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
    QString uploadSpeed   () const;
    QString downloadSpeed () const;


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
                      QString a_uploadSpeed,
                      QString a_downloadSpeed);

private:
    QDateTime   m_startTime {};

    quint64 m_bytesReceived     = 0;
    quint64 m_bytesSent         = 0;
    quint64 m_packetsReceived   = 0;
    quint64 m_packetsSent       = 0;
    QString m_uploadSpeed     = 0;
    QString m_downloadSpeed   = 0;

    DapServerInfo m_serverInfo;

    QString convertByte(const quint64 &bytes_new);
};

#endif // DAPCONNECTIONDATA_H
