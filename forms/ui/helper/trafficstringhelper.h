#ifndef TRAFFICSTRINGHELPER_H
#define TRAFFICSTRINGHELPER_H

#include <QtCore>

class TrafficStringHelper
{
  quint64 m_bytes;
public:
  TrafficStringHelper (quint64 bytes);

  QString asString();
};

#endif // TRAFFICSTRINGHELPER_H
