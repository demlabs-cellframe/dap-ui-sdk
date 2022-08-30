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

    enum connectionQuality {
      NO_CONNECTION = 0,
      FULL,
      ALMOST_FULL,
      MIDDLE,
      LOW,
      VERY_LOW
    };

    DapServerInfo(){}
    DapServerInfo(const DapServerInfo&cp) {
        address     = cp.address;
        address6    = cp.address6;
        port        = cp.port;
        name        = cp.name;
        location    = cp.location;
        online      = cp.online;
        ping        = cp.ping;
        connection_quality = cp.connection_quality;
    }

    QString address;
    QString address6;
    quint16 port = 0;
    QString name;
    QString location;
    QString online;
    int ping = -1;
    connectionQuality connection_quality = NO_CONNECTION;

    bool isAuto() const;
    bool isValid() const;
    bool isOnline() const;

    static bool fromJSON(const QJsonArray& jsonArr, DapServerInfoList& out);
    static bool fromJSON(const QJsonObject& jsonObj, DapServerInfo& out);
    static void sortServerList(QList<DapServerInfo> &serverList);
    static void addGeneralLocation(QList<DapServerInfo> &serverList, QList<DapServerInfo> &generalServerList, const QString base_location, const QString base_location_code);

    static QJsonObject toJSON(const DapServerInfo& dsi);

    friend bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs);
    friend bool operator>(const DapServerInfo& lhs, const DapServerInfo& rhs) { return lhs.ping > rhs.ping; }
    friend bool operator<(const DapServerInfo& lhs, const DapServerInfo& rhs) { return lhs.ping < rhs.ping; }
    friend QDebug operator<< (QDebug out, const DapServerInfo &dsi) {
      out <<  endl << " name: " << dsi.name << " location: "
          << dsi.location << " address: " << dsi.address <<  ":" << dsi.port << " state: " << dsi.online << " ping: " << dsi.ping;
        return out;
    }

    void setPing(const quint16 a_ping){
      ping = a_ping;}
    void setConnetionQulity(int a_connection_quality){
      connection_quality = DAP_CONNECTION_QUALITY(a_connection_quality);
    }

private:
    static bool _isJsonValid(const QJsonObject& obj);
};

Q_DECLARE_METATYPE(DapServerInfo)
typedef DapServerInfo* DapServerInfoPtr;
Q_DECLARE_METATYPE(DapServerInfoPtr)

#endif // DAPSERVERINFO_H
