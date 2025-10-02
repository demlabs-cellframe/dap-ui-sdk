/* INCLUDES */
#include "DapServerInfo.h"

#include <QMetaEnum>

/********************************************
 *
 * DapServerInfo
 *
 *******************************************/



/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapServerInfo::DapServerInfo()
  : m_port (0)
  , m_ping (-1)
  , m_connQuality (ConnectionQuality::NO_CONNECTION)
{

}

DapServerInfo::DapServerInfo(const QJsonObject &a_src)
  : m_port (0)
  , m_ping (-1)
  , m_connQuality (ConnectionQuality::NO_CONNECTION)
{
  fromJSON (a_src, *this);
}

DapServerInfo::DapServerInfo (const DapServerInfo &a_src)
{
  m_address     = a_src.m_address;
  m_address6    = a_src.m_address6;
  m_port        = a_src.m_port;
  m_name        = a_src.m_name;
  m_location    = a_src.m_location;
  m_online      = a_src.m_online;
  m_ping        = a_src.m_ping;
  m_connQuality = a_src.m_connQuality;
  m_order_hash  = a_src.m_order_hash;
}

DapServerInfo::DapServerInfo (DapServerInfo &&a_src)
{
  m_address     = std::move (a_src.m_address);
  m_address6    = std::move (a_src.m_address6);
  m_port        = a_src.m_port;
  m_name        = std::move (a_src.m_name);
  m_location    = std::move (a_src.m_location);
  m_online      = std::move (a_src.m_online);
  m_ping        = a_src.m_ping;
  m_connQuality = a_src.m_connQuality;
  m_order_hash  = std::move (a_src.m_order_hash);
}

DapServerInfo::DapServerInfo (
  QString a_location,
  QString a_name,
  QString a_address,
  quint16 a_port)
  : m_address (a_address)
  , m_port (a_port)
  , m_name (a_name)
  , m_location (a_location)
  , m_ping (-1)
  , m_connQuality (ConnectionQuality::INVALID)
{

}

/********************************************
 * METHODS
 *******************************************/

bool DapServerInfo::fromJSON (const QJsonArray &jsonArr, DapServerInfoList &out)
{
  for (auto it = jsonArr.constBegin(); it != jsonArr.constEnd(); ++it)
    {
      const QJsonValue &val = *it;
      if (!val.isObject())
        {
          qCritical() << "Error parse QJsonValue to QJsonObject";
          return false;
        }

      DapServerInfo dsi;
      if (fromJSON (val.toObject(), dsi) == false)
        {
          continue;
        }

      if (!dsi.m_name.contains ("."))
        {
          continue;
        }

      out.append (std::move (dsi));
    }

  return !out.isEmpty();
}

bool DapServerInfo::fromJSON (const QJsonObject &jsonObj, DapServerInfo &out)
{
  if (_isJsonValid (jsonObj) == false)
    {
      qWarning() << "Invalid json object";
      return false;
    }

  out.m_order_hash = jsonObj["OrderHash"].toString();
  out.m_address = jsonObj["Address"].toString();
  out.m_address6 = jsonObj["Address6"].toString();
  out.m_port = quint16 (jsonObj["Port"].toInt());
  out.m_name = jsonObj["Name"].toString();
  out.m_online = jsonObj["State"].toString();
  if (out.m_name == "Auto")
    {
      out.m_location = "UNKNOWN";
      return true;
    }
  out.m_location = jsonObj["Location"].toString();

  return true;
}

QJsonObject DapServerInfo::toJSON (const DapServerInfo &dsi)
{
  QJsonObject obj;
  obj["Address"] = dsi.m_address;
  obj["Address6"] = dsi.m_address6;
  obj["Port"] = dsi.m_port;
  obj["Name"] = dsi.m_name;
  obj["State"] = dsi.m_online;
  obj["Location"] = dsi.m_location;
  obj["OrderHash"] = dsi.m_order_hash;
  return obj;
}

QJsonObject DapServerInfo::toJSON() const
{
  return toJSON (*this);
}

void DapServerInfo::sortServerList (QList<DapServerInfo> &serverList)
{
  std::sort(serverList.begin(), serverList.end());

  QList<DapServerInfo> availableServerList, notAvailableServerList;

  //unavailable at the end of the list
  for (auto &server : serverList)
    {

      if (server.m_name == "Auto")
        {
          notAvailableServerList.push_front (server);
          continue;
        }

      if (server.m_ping == -1)
        {
          availableServerList.push_back (server);
        }
      else
        {
          notAvailableServerList.push_back (server);
        }
    }

  serverList.clear();
  serverList = notAvailableServerList += availableServerList;
}

void DapServerInfo::addGeneralLocation (
  const QList<DapServerInfo> &pingServerList,
  QList<DapServerInfo> &bestRegionServers,
  const QString a_location)
{
  QSet <QString> general_location;

  if (bestRegionServers.isEmpty())
    {
      general_location << "Auto";

      for (auto &server : pingServerList)
        {
          general_location << server.m_name.left (server.m_name.indexOf ('.'));
        }

      for (auto &location : general_location)
        {
          DapServerInfo d_server;
          d_server.m_name = location;
          bestRegionServers.push_back (d_server);
        }
    }

  for (auto &region_server : bestRegionServers)
    {
      for (auto server : pingServerList)
        {
          if (region_server.m_name == server.m_name.left (server.m_name.indexOf ('.')) && server.m_location != a_location)
            {
              QString tmp_name = region_server.m_name;
              region_server = server;
              region_server.m_name = tmp_name;
              break;
            }
        }
    }

  DapServerInfo::sortServerList (bestRegionServers);

  for (auto &region_server : bestRegionServers)
    {
      for (auto server : pingServerList)
        {
          if (region_server.m_name == "Auto" && server.m_location != a_location)
            {
              region_server = server;
              region_server.m_name = "Auto";
              bestRegionServers.move (bestRegionServers.indexOf (region_server), 0);
              break;
            }
        }
    }
}

const QString &DapServerInfo::address() const
{
  return m_address;
}

void DapServerInfo::setAddress (const QString &address)
{
  m_address = address;
}

const QString &DapServerInfo::orderHash() const
{
  return m_order_hash;
}

void DapServerInfo::setOrderHash (const QString &hash)
{
  m_order_hash = hash;
}

const QString &DapServerInfo::address6() const
{
  return m_address6;
}

void DapServerInfo::setAddress6 (const QString &address6)
{
  m_address6 = address6;
}

quint16 DapServerInfo::port() const
{
  return m_port;
}

void DapServerInfo::setPort (const quint16 &port)
{
  m_port = port;
}

const QString &DapServerInfo::name() const
{
  return m_name;
}

void DapServerInfo::setName (const QString &name)
{
  m_name = name;
}

const QString &DapServerInfo::location() const
{
  return m_location;
}

void DapServerInfo::setLocation (const QString &location)
{
  m_location = location;
}

const QString &DapServerInfo::online() const
{
  return m_online;
}

void DapServerInfo::setOnline (const QString &online)
{
  m_online = online;
}

int DapServerInfo::ping() const
{
  return m_ping;
}

void DapServerInfo::setPing (int ping)
{
  m_ping = ping;
}

DapServerInfo::ConnectionQuality DapServerInfo::connQuality() const
{
  return m_connQuality;
}

void DapServerInfo::setConnQuality (const ConnectionQuality &connQuality)
{
  m_connQuality = connQuality;
//    (int (connQuality) >= int (ConnectionQuality::NO_CONNECTION))
//    ? connQuality
//    : ConnectionQuality::NO_CONNECTION;
}

bool DapServerInfo::isAuto() const
{
  return (this->m_name == "Auto");
}

bool DapServerInfo::isValid() const
{
  return (!m_address.isEmpty());
}

bool DapServerInfo::isOnline() const
{
  return (this->m_online == "available");
}

QVariant DapServerInfo::value (int a_fieldId) const
{
  switch (DapServerType::FieldId (a_fieldId))
    {
    case DapServerType::FieldId::name:        return name();
    case DapServerType::FieldId::address:     return address();
    case DapServerType::FieldId::address6:    return address6();
    case DapServerType::FieldId::port:        return port();
    case DapServerType::FieldId::location:    return location();
    case DapServerType::FieldId::online:      return online();
    case DapServerType::FieldId::ping:        return ping();
    case DapServerType::FieldId::connQuality: return int (connQuality());
    }
  return QVariant();
}

bool DapServerInfo::_isJsonValid (const QJsonObject &obj)
{
  auto contains = [&] (const char *key)
  {
    return obj.contains (key);
  };

  return (contains ("Address") || contains ("Address6")) && contains ("Name") && contains ("Port")
         && obj["Port"].isDouble();
}

/********************************************
 * OPERATORS
 *******************************************/

bool operator== (const DapServerInfo &lhs, const DapServerInfo &rhs)
{
  if (lhs.m_address == rhs.m_address && lhs.m_address6 == rhs.m_address6 && lhs.m_port == rhs.m_port)
    {
      return true;
    }
  return false;
}

DapServerInfo &DapServerInfo::operator= (const DapServerInfo &a_src)
{
  m_address   = a_src.m_address;
  m_address6  = a_src.m_address6;
  m_port      = a_src.m_port;
  m_name      = a_src.m_name;
  m_location  = a_src.m_location;
  m_online    = a_src.m_online;
  m_ping      = a_src.m_ping;
  m_connQuality  = a_src.m_connQuality;
  m_order_hash = a_src.m_order_hash;
  return *this;
}

DapServerInfo &DapServerInfo::operator= (DapServerInfo &&a_src)
{
  m_address   = std::move (a_src.m_address);
  m_address6  = std::move (a_src.m_address6);
  m_port      = a_src.m_port;
  m_name      = std::move (a_src.m_name);
  m_location  = std::move (a_src.m_location);
  m_online    = std::move (a_src.m_online);
  m_ping      = a_src.m_ping;
  m_connQuality  = a_src.m_connQuality;
  m_order_hash = std::move (a_src.m_order_hash);
  return *this;
}



/********************************************
 *
 * DapServerType
 *
 *******************************************/



/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapServerType::DapServerType()
  : QObject()
  , DapServerInfo()
{

}

DapServerType::DapServerType (const DapServerInfo &a_src)
  : QObject()
  , DapServerInfo (a_src)
{

}

DapServerType::DapServerType (DapServerType &&a_src)
  : QObject()
  , DapServerInfo (std::move (a_src))
{

}

/********************************************
 * METHODS
 *******************************************/

static QMap<DapServerType::FieldId, QString> _metaEnumToMap (const char *a_name, const QMetaObject &a_metaObject)
{
  QMap<DapServerType::FieldId, QString> result;

//  for (int i = 0; i < a_metaObject.enumeratorCount(); i++)
//    {
//      auto name = a_metaObject.enumerator (i).enumName();
//      qDebug() << name;
//    }

  int enumIndex  = a_metaObject.indexOfEnumerator (a_name);
  auto metaEnum  = a_metaObject.enumerator (enumIndex);
  int enumSize   = metaEnum.keyCount();

  for (int i = 0; i < enumSize; i++)
    result.insert (DapServerType::FieldId (metaEnum.value (i)), metaEnum.key (i));

  return result;
}

QVariant DapServerType::asVariantMap() const
{
  static auto fieldMap  = _metaEnumToMap ("FieldId", DapServerType::staticMetaObject);

  QVariantMap result;

  for (auto i = fieldMap.cbegin(), e = fieldMap.cend(); i != e; i++)
    result.insert (i.value(), DapServerInfo::value (i.key()));

  return result;
}

/*-----------------------------------------*/
