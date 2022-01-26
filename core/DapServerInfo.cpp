#include "DapServerInfo.h"



QString DapServerInfo::stringToLocation(const QString& location) {
    return location.toUpper();
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

bool DapServerInfo::isAuto() const
{
    return (this->name == "Auto");
}

bool DapServerInfo::isOnline() const
{
    return (this->online == "available");
}

bool DapServerInfo::isValid() const
{
    return (!address.isEmpty());
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
            continue;
        }
        out.append(std::move(dsi));
    }

    return !out.isEmpty();
}

QJsonObject DapServerInfo::toJSON(const DapServerInfo& dsi)
{
    QJsonObject obj;
    obj["Address"] = dsi.address;
    obj["Address6"] = dsi.address6;
    obj["Port"] = dsi.port;
    obj["Name"] = dsi.name;
    obj["State"] = dsi.online;
    obj["Location"] = dsi.location;
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
    out.online = jsonObj["State"].toString();
    if (out.name == "Auto"){
        out.location = "UNKNOWN";
        return true;
    }
    out.location = jsonObj["Location"].toString();

    return true;
}
