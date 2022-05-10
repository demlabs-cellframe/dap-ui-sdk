#include "DapCmdSystemTray.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdSystemTray::actionParam = "client_show_interface";

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
    response["client_show_interface"] = interfaceName;
    sendCmd(&response);
}


void DapCmdSystemTray::handle(const QJsonObject* params) {

    if(params->value(actionParam) != QJsonValue::Undefined)
    {
        emit interfaceRequest(params->value(actionParam).toString());
        return;
    }
}
