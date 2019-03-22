#ifndef DAPSERVERINFO_H
#define DAPSERVERINFO_H

#include <QString>
#include <QMap>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

enum class DapServerLocation {
    ENGLAND = 1,
    FRANCE,
    GERMANY,
    USA,
    NETHERLANDS,
    RUSSIA,
    UKRAINE,
    UNKNOWN
};

class DapServerInfo;
using DapServerInfoList = QVector<DapServerInfo>;

class DapServerInfo
{
public:
    using countryMap = QMap<QString, DapServerLocation>;
    DapServerInfo(){}
    DapServerInfo(const DapServerInfo&cp) {
        address     = cp.address;
        port        = cp.port;
        name        = cp.name;
        location    = cp.location;
    }

    QString address;
    quint16 port;
    QString name;
    DapServerLocation location;

    static bool parseJSON(QJsonArray jsonArr, DapServerInfoList& out);
    static bool parseJSON(QJsonObject jsonObj, DapServerInfo& out);

    static DapServerLocation stringToLaction(const QString& location);
    friend bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs);
    friend QDebug operator<< (QDebug out, const DapServerInfo &dsi) {
        out << "DapServer address:" << dsi.address << "port:" << dsi.port
            << "name:"    << dsi.name    << "location:"
            << m_countries.key(dsi.location);
        return out;
    }
private:
    static bool _isJsonValid(const QJsonObject& obj);
    static countryMap m_countries;
};

#endif // DAPSERVERINFO_H
