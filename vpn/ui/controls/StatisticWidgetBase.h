#ifndef STATISTICWIDGETBASE_H
#define STATISTICWIDGETBASE_H

#include <QDateTime>

class StatisticWidgetBase
{
public:
    StatisticWidgetBase(){};
    virtual ~StatisticWidgetBase(){};

    virtual void setSentReceivedQuantity(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent) = 0;

    virtual void setLoginTime(const QDateTime &loginTime) = 0;

    virtual void setConnectingTimePoint(const QDateTime &loginTime) = 0; //When we were connected (is not duration)

    virtual void setUploadSpeedText(const QString& a_speedString) = 0;
    virtual void setDownloadSpeedText(const QString& a_speedString) = 0;
};

#endif // STATISTICWIDGETBASE_H
