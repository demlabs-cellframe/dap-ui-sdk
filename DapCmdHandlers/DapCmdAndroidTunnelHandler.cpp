#include "DapCmdAndroidTunnelHandler.h"
#include "DapJsonCmd.h"

const QString DapCmdAndroidTunnelHandler::addressParam = DapJsonParams::toString(DapJsonParams::ADDRESS);
const QString DapCmdAndroidTunnelHandler::gwParam = DapJsonParams::toString(DapJsonParams::GATEWAY);
const QString DapCmdAndroidTunnelHandler::upstreamParam = DapJsonParams::toString(DapJsonParams::UPSTREAM_SOCKET);

void DapCmdAndroidTunnelHandler::handler(const QJsonObject *params)
{
    if (params->value(addressParam) != QJsonValue::Undefined && params->value(gwParam) != QJsonValue::Undefined &&
            params->value(upstreamParam) != QJsonValue::Undefined) {
        tunnelHandler(params->value(addressParam), params->value(gwParam), params->value(upstreamParam));
    }
}

void DapCmdAndroidTunnelHandler::tunnelHandler(const QJsonValue &address, const QJsonValue &gateway, const QJsonValue &upstreamSocket)
{
    auto addr = address.toString();
    auto gw = gateway.toString();
    auto fd = upstreamSocket.toInt();

    if (addr.isNull() || addr.isEmpty() || gw.isNull() || gw.isEmpty()) {
        qWarning() << "Something wrong can't parse creating android tunnel";
        return;
    }

    emit sigTunnelCreate(addr, gw, fd);
}
