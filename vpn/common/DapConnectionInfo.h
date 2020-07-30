#ifndef DAPCONNECTIONDATA_H
#define DAPCONNECTIONDATA_H

#include <QObject>

class DapConnectionData : public QObject
{
    Q_OBJECT
    explicit DapConnectionData(QObject *parent = nullptr); //private constructor becouse it's can't be create outside Class

public:
    ~DapConnectionData();

    static DapConnectionData* curentConnection();

    QDateTime   m_connectionStartTime;
    int         m_bytesReceived;
    int         m_bytesSent;
    int         m_packetsReceived;
    int         m_packetsSent;


private:
    DapConnectionData* m_currentConnection = nullptr;

};

#endif // DAPCONNECTIONDATA_H
