#ifndef DAPSERVERINFO_H
#define DAPSERVERINFO_H

#include <QString>
#include <QMap>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

class DapServerInfo;
using DapServerInfoList = QVector<DapServerInfo>;

class DapServerInfo
{
public:
    DapServerInfo(){}
    DapServerInfo(const DapServerInfo&cp) {
        address     = cp.address;
        address6    = cp.address6;
        port        = cp.port;
        name        = cp.name;
        location    = cp.location;
        online      = cp.online;
    }

    QString address;
    QString address6;
    quint16 port = 0;
    QString name;
    QString location;
    QString online;

    bool isAuto() const;
    bool isValid() const;
    bool isOnline() const;

    static bool fromJSON(const QJsonArray& jsonArr, DapServerInfoList& out);
    static bool fromJSON(const QJsonObject& jsonObj, DapServerInfo& out);

    static QJsonObject toJSON(const DapServerInfo& dsi);

    static QString stringToLocation(const QString& location);
    friend bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs);
    friend QDebug operator<< (QDebug out, const DapServerInfo &dsi) {
        out << "DapServer address:" << dsi.address << " address6:" << dsi.address6 << "port:" << dsi.port
            << "name:"    << dsi.name    << "location:"
            << dsi.location << "state: " << dsi.online;
        return out;
    }

private:
    static bool _isJsonValid(const QJsonObject& obj);
};

Q_DECLARE_METATYPE(DapServerInfo)
typedef DapServerInfo* DapServerInfoPtr;
Q_DECLARE_METATYPE(DapServerInfoPtr)

#endif // DAPSERVERINFO_H
