#ifndef DAPSERVERINFO_H
#define DAPSERVERINFO_H

#include <QString>
#include <QMap>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

class DapServerInfo;
using DapServerInfoList = QVector<DapServerInfo>;

#define DAP_CONNECTION_QUALITY DapServerInfo::connectionQuality

class DapServerInfo
{
public:

    enum class ConnectionQuality {
      NO_CONNECTION = 0,
      FULL,
      ALMOST_FULL,
      MIDDLE,
      LOW,
      VERY_LOW
    };

    DapServerInfo () {}
    DapServerInfo (const DapServerInfo &a_src) {
        address     = a_src.address;
        address6    = a_src.address6;
        port        = a_src.port;
        name        = a_src.name;
        location    = a_src.location;
        online      = a_src.online;
        ping        = a_src.ping;
        connection_quality = a_src.connection_quality;
        _flags      = a_src._flags;
    }
    DapServerInfo (DapServerInfo &&a_src) {
      address     = std::move (a_src.address);
      address6    = std::move (a_src.address6);
      port        = a_src.port;
      name        = std::move (a_src.name);
      location    = std::move (a_src.location);
      online      = std::move (a_src.online);
      ping        = a_src.ping;
      connection_quality = a_src.connection_quality;
      _flags      = a_src._flags;
    }

    QString address;
    QString address6;
    quint16 port = 0;
    QString name;
    QString location;
    QString online;
    int ping = -1;
    ConnectionQuality connection_quality = ConnectionQuality::NO_CONNECTION;
    union {
      quint32 _flags = 0;   // flags bits combined
      struct {
        bool flagAuto : 1;  // auto server
        bool flagUser : 1;  // user defined server
      };
    };

    bool isAuto() const;
    bool isUserDefined() const;
    bool isValid() const;
    bool isOnline() const;

    static bool fromJSON(const QJsonArray& jsonArr, DapServerInfoList& out);
    static bool fromJSON(const QJsonObject& jsonObj, DapServerInfo& out);
    static void sortServerList(QList<DapServerInfo> &serverList);
    static void addGeneralLocation(QList<DapServerInfo> &serverList, QList<DapServerInfo> &generalServerList, QString base_location);

    static QJsonObject toJSON(const DapServerInfo& dsi);

    friend bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs);
    friend bool operator>(const DapServerInfo& lhs, const DapServerInfo& rhs) { return lhs.ping > rhs.ping; }
    friend bool operator<(const DapServerInfo& lhs, const DapServerInfo& rhs) { return lhs.ping < rhs.ping; }
    friend QDebug operator<< (QDebug out, const DapServerInfo &dsi) {
        out <<  endl
            << " name: " << dsi.name
            << " location: " << dsi.location
            << " address: " << dsi.address
            <<  ":" << dsi.port
            << " state: " << dsi.online
            << " ping: " << dsi.ping
            << " flags: auto " << dsi.flagAuto
            << " userdef " << dsi.flagUser;
        return out;
    }

    void setPing (const quint16 a_ping)
    {
      ping = a_ping;
    }

    void setConnetionQulity (int a_connection_quality)
    {
      connection_quality =
          (a_connection_quality >= int (ConnectionQuality::NO_CONNECTION))
          ? ConnectionQuality (a_connection_quality)
          : ConnectionQuality::NO_CONNECTION;
    }

    DapServerInfo &operator= (const DapServerInfo &a_src)
    {
      address     = a_src.address;
      address6    = a_src.address6;
      port        = a_src.port;
      name        = a_src.name;
      location    = a_src.location;
      online      = a_src.online;
      ping        = a_src.ping;
      connection_quality = a_src.connection_quality;
      _flags      = a_src._flags;
      return *this;
    }

    DapServerInfo &operator= (DapServerInfo &&a_src)
    {
      address     = std::move (a_src.address);
      address6    = std::move (a_src.address6);
      port        = a_src.port;
      name        = std::move (a_src.name);
      location    = std::move (a_src.location);
      online      = std::move (a_src.online);
      ping        = a_src.ping;
      connection_quality = a_src.connection_quality;
      _flags      = a_src._flags;
      return *this;
    }

private:
    static bool _isJsonValid(const QJsonObject& obj);
};

Q_DECLARE_METATYPE(DapServerInfo)
typedef DapServerInfo* DapServerInfoPtr;
Q_DECLARE_METATYPE(DapServerInfoPtr)

#endif // DAPSERVERINFO_H
