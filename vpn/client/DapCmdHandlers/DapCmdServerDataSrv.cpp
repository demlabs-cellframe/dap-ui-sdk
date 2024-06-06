#include "DapCmdServerDataSrv.h"
#include <QDebug>
#include <QMap>

void DapCmdServerDataSrv::handle(const QJsonObject *params)
{
    if (params->value("address") != QJsonValue::Undefined
            && params->value("port") != QJsonValue::Undefined)
    {
        emit sigChangeServerData(params->value("address").toString(), ushort(params->value("port").toInt()));
    }
}

void DapCmdServerDataSrv::sendServerInfo(const DapServerInfo &server)
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
