#include "DapCmdTunTap.h"

void DapCmdTunTap::sendCmd(const QString& val)
{
    static QJsonObject result;
    result["value"] = val;
    DapCmdServiceAbstract::sendCmd(&result);
}

void DapCmdTunTap::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    if (params->value("action").toString() == "check") {
        emit sigTapCheck();
    } else if (params->value("action").toString() == "install") {
        emit sigTapInstall();
    }
}
