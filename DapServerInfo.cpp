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

DapServerLocation DapServerInfo::stringToLaction(const QString& location) {
    DapServerLocation v = m_countries.value(location.toUpper());
    if (int(v) == 0) {
        qWarning() << "Unknown location" << location;
        return DapServerLocation::UNKNOWN;
    }
    return v;
}

bool operator==(const DapServerInfo& lhs, const DapServerInfo& rhs)
{
    if(lhs.address == rhs.address && lhs.port == rhs.port) {
        return true;
    }
    return false;
}

bool DapServerInfo::_isJsonValid(const QJsonObject& obj)
{
    auto contains = [&](const char* key) {
        return obj.contains(key);
    };

    return contains("Address") && contains("Name") && contains("Port")
            && obj["Port"].isDouble();
}

bool DapServerInfo::parseJSON(QJsonArray jsonArr, DapServerInfoList& out)
{
    for (auto it = jsonArr.constBegin(); it != jsonArr.constEnd(); ++it)
    {
        const QJsonValue &val = *it;

        if(!val.isObject()) {
            qCritical() << "Error parse QJsonValue to QJsonObject";
            return false;
        }

        DapServerInfo dsi;
        if(parseJSON(val.toObject(), dsi) == false) {
            return false;
        }

        out.append(std::move(dsi));
    }
    return true;
}

bool DapServerInfo::parseJSON(QJsonObject jsonObj, DapServerInfo& out)
{
    if(_isJsonValid(jsonObj) == false) {
        qWarning() << "Invalid json object";
        return false;
    }

    out.address = jsonObj["Address"].toString();
    out.port = quint16(jsonObj["Port"].toInt());
    out.name = jsonObj["Name"].toString();

    QString countryName = jsonObj["Country"].toObject()["Name"].toString();
    out.location = stringToLaction(countryName);
    return true;
}
