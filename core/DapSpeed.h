#ifndef DAPSPEED_H
#define DAPSPEED_H

#include <QtCore>

quint64 CurrentMillisecond();
QString SpeedToString(quint64 a_speed);

class DapSpeed
{
private:
    quint64 m_bytes;
    quint64 m_millisecond;
    quint64 m_speed;
    bool    m_init;
    // return current time in ms
    quint64 currentMillisecond();
public:
    DapSpeed ();
    void setTraffic(quint64 bytes);
    quint64 speed() const;
    quint64 traffic() const;
    QString asString() const;
    void reset();
};

/*-----------------------------------------*/
#endif // DAPSPEED_H
