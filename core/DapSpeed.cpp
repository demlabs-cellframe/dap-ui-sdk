/* INCLUDES */
#include "DapSpeed.h"

quint64 CurrentMillisecond()
{
    quint64 ctime = (quint64) time(nullptr);
    QTime qttime = QTime().currentTime();
    // Millisecond = second * 1000 + msec
    return ctime * 1000 + qttime.msec();
}


QString SpeedToString(quint64 a_speed)
{
    if (a_speed >= 1E6)
    {
        double speed = a_speed/1E6;
        int digitCount = 0;
        if ((10 > speed) && (speed >= 0))
            digitCount = 2;
        if ((100 > speed) && (speed >= 10))
            digitCount = 1;
        return QString("%1 %2").arg(QString::number(speed, 'f', digitCount)).arg("Mb/s");
    }
    else
    {
        double speed = a_speed/1E3;
        int digitCount = 0;
        if ((10 > speed) && (speed >= 0))
            digitCount = 2;
        if ((100 > speed) && (speed >= 10))
            digitCount = 1;
        return QString("%1 %2").arg(QString::number(speed, 'f', digitCount)).arg("Kb/s");
    }
}


DapSpeed::DapSpeed ()
  : m_bytes (0)
  , m_millisecond (0)
  , m_speed (0)
  , m_init (false)
{

}

/********************************************
 * METHODS
 *******************************************/

QString DapSpeed::asString() const
{
    return SpeedToString(m_speed);
}

void DapSpeed::setTraffic(quint64 bytes)
{
    quint64 a_millisecond = currentMillisecond();
    if (!m_init)
    {
        m_speed = 0;
        m_init = true;
    }
    else
    {
        if (a_millisecond != m_millisecond)
        {
            double dtime = (a_millisecond - m_millisecond) * 1E-3;
            m_speed = (bytes - m_bytes)/dtime;
        }
    }
    m_millisecond = a_millisecond;
    m_bytes = bytes;
}

quint64 DapSpeed::speed() const
{
    return m_speed;
}

quint64 DapSpeed::traffic() const
{
    return m_bytes;
}

void DapSpeed::reset()
{
    m_init = false;
    m_speed = 0;
}

quint64 DapSpeed::currentMillisecond()
{
    return CurrentMillisecond();
}
