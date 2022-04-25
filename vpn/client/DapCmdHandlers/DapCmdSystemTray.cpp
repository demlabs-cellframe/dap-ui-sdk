#include "DapCmdSystemTray.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdSystemTray::actionParam = "clientShowInterface";

DapCmdSystemTray::DapCmdSystemTray(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::CLIENT_INFO, parent) {
    connect(this, &DapCmdSystemTray::interfaceRequest, this, &DapCmdSystemTray::sendInterfaceRequest);
}

DapCmdSystemTray::~DapCmdSystemTray()
{

}

/**
 * @brief DapCmdConnect::sendCmdError
 * @param interfaceName
 */
void DapCmdSystemTray::sendInterfaceRequest(QString interfaceName)
{
    QJsonObject response;
    response["clientShowInterface"] = interfaceName;
    sendCmd(&response);
}


void DapCmdSystemTray::handle(const QJsonObject* params) {

    if(params->value(actionParam) != QJsonValue::Undefined)
    {
        emit interfaceRequest(params->value(actionParam).toString());
        return;
    }
}
