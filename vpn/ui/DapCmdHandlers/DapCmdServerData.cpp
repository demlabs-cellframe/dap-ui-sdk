#include "DapCmdServerData.h"

void DapCmdServerData::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
}

/// @param address Server address.
void DapCmdServerData::sendServerData(const QString &address, quint16 a_port)
{
    QJsonObject response;
    response["address"] = address;
    response["port"] = a_port;
    sendCmd(&response);
}
