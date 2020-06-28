#include "DapCmdServerData.h"
#include <QDebug>
#include <QMap>

void DapCmdServerData::handle(const QJsonObject *params) {
    if (params->value("address") != QJsonValue::Undefined
            && params->value("port") != QJsonValue::Undefined)
    {
        emit sigChangeServerData(params->value("address").toString(), ushort(params->value("port").toInt()));
    }
}
