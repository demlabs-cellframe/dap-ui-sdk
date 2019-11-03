#include "DapServerInfo.h"

DapServerInfo::countryMap DapServerInfo::m_countries = {
    {"ENGLAND", DapServerLocation::ENGLAND},
    {"FRANCE", DapServerLocation::FRANCE},
    {"GERMANY", DapServerLocation::GERMANY},
    {"USA", DapServerLocation::USA},
    {"NETHERLANDS", DapServerLocation::NETHERLANDS},
    {"RUSSIA", DapServerLocation::RUSSIA},
    {"UKRAINE", DapServerLocation::UKRAINE},
    {"UNKNOWN", DapServerLocation::UNKNOWN}
};

DapServerInfo::countryMap2 DapServerInfo::m_countries2 = {
    {DapServerLocation::ENGLAND, "ENGLAND", },
    {DapServerLocation::FRANCE,"FRANCE" },
    {DapServerLocation::GERMANY,"GERMANY" },
    {DapServerLocation::USA,"USA" },
    {DapServerLocation::NETHERLANDS,"NETHERLANDS" },
    {DapServerLocation::RUSSIA,"RUSSIA" },
    {DapServerLocation::UKRAINE,"UKRAINE" },
    {DapServerLocation::UNKNOWN,"UNKNOWN"}
};

DapServerLocation DapServerInfo::stringToLocation(const QString& location) {
    DapServerLocation v = m_countries.value(location.toUpper());
    if (int(v) == 0) {
        qWarning() << "Unknown location" << location;
        return DapServerLocation::UNKNOWN;
    }
    return v;
}

bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs)
{
    if( lhs.address == rhs.address && lhs.address6 == rhs.address6 && lhs.port == rhs.port) {
        return true;
    }
    return false;
}

bool DapServerInfo::_isJsonValid(const QJsonObject& obj)
{
    auto contains = [&](const char* key) {
        return obj.contains(key);
    };

    return (contains("Address") || contains("Address6") ) && contains("Name") && contains("Port")
            && obj["Port"].isDouble();
}

bool DapServerInfo::fromJSON(const QJsonArray& jsonArr, DapServerInfoList& out)
{
    for (auto it = jsonArr.constBegin(); it != jsonArr.constEnd(); ++it)
    {
        const QJsonValue &val = *it;

        if(!val.isObject()) {
            qCritical() << "Error parse QJsonValue to QJsonObject";
            return false;
        }

        DapServerInfo dsi;
        if(fromJSON(val.toObject(), dsi) == false) {
            return false;
        }

        out.append(std::move(dsi));
    }
    return true;
}

QJsonObject DapServerInfo::toJSON(const DapServerInfo& dsi)
{
    QJsonObject obj;
    obj["Address"] = dsi.address;
    obj["Address6"] = dsi.address6;
    obj["Port"] = dsi.port;
    obj["Name"] = dsi.name;
    obj["Location"] =  DapServerInfo::m_countries2[ dsi.location];
    return obj;
}

bool DapServerInfo::fromJSON(const QJsonObject& jsonObj, DapServerInfo& out)
{
    if(_isJsonValid(jsonObj) == false) {
        qWarning() << "Invalid json object";
        return false;
    }

    out.address = jsonObj["Address"].toString();
    out.address6 = jsonObj["Address6"].toString();
    out.port = quint16(jsonObj["Port"].toInt());
    out.name = jsonObj["Name"].toString();
    out.location = m_countries[ jsonObj["Location"].toString() ];

    return true;
}
