#include "trafficstringhelper.h"

TrafficStringHelper::TrafficStringHelper (quint64 bytes)
  : m_bytes (bytes)
{

}

QString TrafficStringHelper::asString()
{
  if (m_bytes < 1024)
    return QString ("%1 Bytes").arg (m_bytes);
  if (m_bytes < 1024 * 1024)
    return QString ("%1 Kb").arg (m_bytes / 1024);
  if (m_bytes < 1024 * 1024 * 1024)
    return QString ("%1 Mb").arg (m_bytes / (1024 * 1024));
  return QString ("%1 Gb").arg (m_bytes / (1024 * 1024 * 1024));
}
