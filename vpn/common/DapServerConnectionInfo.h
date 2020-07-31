#ifndef DAPCONNECTIONDATA_H
#define DAPCONNECTIONDATA_H

#include <QObject>
#include <QDateTime>

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
    DapServerConnectionInfo* m_currentConnection = nullptr;

    QDateTime   m_startTime {};
    int         m_bytesReceived = 0;
    int         m_bytesSent = 0;
    int         m_packetsReceived = 0;
    int         m_packetsSent = 0;


};

#endif // DAPCONNECTIONDATA_H
