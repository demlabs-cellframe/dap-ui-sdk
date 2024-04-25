#include "DapCmdTunTapSrv.h"

void DapCmdTunTapSrv::sendCmd(const QString& val)
{
    static QJsonObject result;
    result["value"] = val;
    DapCmdServiceAbstract::sendCmd(&result);
}

void DapCmdTunTapSrv::handle(const QJsonObject *params) {
    if (params->value("action").toString() == "check") {
        emit sigTapCheck();
    } else if (params->value("action").toString() == "install") {
        emit sigTapInstall();
    }
}
