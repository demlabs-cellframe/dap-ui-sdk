/* INCLUDES */
#include "trafficstringhelper.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

TrafficStringHelper::TrafficStringHelper (quint64 bytes)
  : m_bytes (bytes)
{

}

/********************************************
 * METHODS
 *******************************************/

QString TrafficStringHelper::asString()
{
  if (m_bytes < 1000)
    return QString ("%1 Bytes").arg (m_bytes);
  if (m_bytes < 1000 * 1000)
    return QString ("%1 Kb").arg (m_bytes / 1000);
  if (m_bytes < 1000 * 1000 * 1000)
    return QString ("%1 Mb").arg (m_bytes / (1000 * 1000));
  return QString ("%1 Gb").arg (m_bytes / (1000 * 1000 * 1000));
}

/*-----------------------------------------*/
