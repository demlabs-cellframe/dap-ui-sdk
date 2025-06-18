/* INCLUDES */
#include "DapCmdSystemTray.h"

#include <QMap>
#include <QDebug>

/* VARS */
static const QString actionParam = "action";

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdSystemTray::DapCmdSystemTray (QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::CLIENT_INFO, parent)
{
    //  connect (this, &DapCmdSystemTray::interfaceRequest,
    //           this, &DapCmdSystemTray::sendInterfaceRequest,
    //           Qt::QueuedConnection);
}

DapCmdSystemTray::~DapCmdSystemTray()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdSystemTray::handle (const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    auto value  = params->value (actionParam);
    if (value != QJsonValue::Undefined)
        //emit interfaceRequest (value.toString());
        sendCmd (params);
}

/********************************************
 * METHODS
 *******************************************/

/**
 * @brief DapCmdConnect::sendCmdError
 * @param interfaceName
 */
//void DapCmdSystemTray::sendInterfaceRequest (QString interfaceName)
//{
//  QJsonObject response = {{actionParam, interfaceName}};
//  sendCmd (&response);
//}

/*-----------------------------------------*/
