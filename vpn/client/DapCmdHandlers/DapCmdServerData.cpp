#include "DapCmdServerData.h"
#include <QDebug>
#include <QMap>

void DapCmdServerData::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    if (params->value("address") != QJsonValue::Undefined
            && params->value("port") != QJsonValue::Undefined)
    {
        emit sigChangeServerData(params->value("address").toString(), ushort(params->value("port").toInt()));
    }
}

void DapCmdServerData::sendServerInfo(const DapServerInfo &server)
{
    QJsonObject obj;
    obj["address"]     = server.address();
    obj["address6"]    = server.address6();
    obj["port"]        = server.port();
    obj["name"]        = server.name();
    obj["location"]    = server.location();
    obj["online"]      = server.online();
    sendCmd(&obj);
}
