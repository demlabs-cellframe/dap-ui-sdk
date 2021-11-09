/* INCLUDES */
#include "uptimestringhelper.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

UptimeStringHelper::UptimeStringHelper (quint64 msec)
  : m_msecs (msec)
{
  sec   = msec / (1000) % 60;
  min   = msec / (1000 * 60) % 60;
  hour  = msec / (1000 * 60 * 60) % 60;
  day   = msec / (1000 * 60 * 60 * 24);
}

/********************************************
 * METHODS
 *******************************************/

QString UptimeStringHelper::asString()
{
  QString r;
  if (day)
    r += QString().asprintf ("%dd", day);
  if (hour)
    r += QString().asprintf ((day) ? "-%02d" : "%d", hour);
  if (min)
    r += QString().asprintf ((hour) ? ":%02d" : "%d", min);
  r += QString().asprintf ((min) ? ":%02d" : "%d", sec);
  return r;
}

/*-----------------------------------------*/
