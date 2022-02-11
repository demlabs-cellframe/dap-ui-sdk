#ifndef SPEEDHELPER_H
#define SPEEDHELPER_H

#include <QtCore>

class SpeedStringHelper
{
private:
    quint64 m_bytes;
    quint64 m_millisecond;
    quint64 m_speed;
    bool    m_init;
    // return current time in ms
    quint64 CurrentMillisecond();
public:
    SpeedStringHelper ();
    void setTraffic(quint64 bytes);
    quint64 speed() const;
    quint64 traffic() const;
    QString asString() const;
    void reset();
};

/*-----------------------------------------*/
#endif // SPEEDHELPER_H
