#ifndef DAPCONNECTIONDATA_H
#define DAPCONNECTIONDATA_H

#include <QObject>
#include <QDateTime>

#include "DapServerInfo.h"

class DapServerConnectionInfo : public QObject
{
    Q_OBJECT

public:
    explicit DapServerConnectionInfo(QObject *parent = nullptr);
    ~DapServerConnectionInfo();

    QDateTime startTime () const;
    int bytesReceived   () const;
    int bytesSent       () const;
    int packetsReceived () const;
    int packetsSent     () const;



    const DapServerInfo *serverInfo() const;
    void setServerInfo(const DapServerInfo &serverInfo);

public slots:
    void setStartTime(const QDateTime &a_startTime);

    void setStatistic(int a_bytesReceived,
                      int a_bytesSent,
                      int a_packetsReceived,
                      int a_packetsSent);

    void clearStatisticAndStartTime();

signals:

    void startTimeChanged(const QDateTime& a_time);
    void statisticSet(int a_bytesReceived,
                      int a_bytesSent,
                      int a_packetsReceived,
                      int a_packetsSent);

private:
    QDateTime   m_startTime {};
    int         m_bytesReceived = 0;
    int         m_bytesSent = 0;
    int         m_packetsReceived = 0;
    int         m_packetsSent = 0;

    DapServerInfo m_serverInfo;

};

#endif // DAPCONNECTIONDATA_H
