/* INCLUDES */
#include "DapCmdSystemTraySrv.h"

#include <QMap>
#include <QDebug>

/* VARS */
static const QString actionParam = "action";

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdSystemTraySrv::DapCmdSystemTraySrv (QObject *parent)
  : DapCmdServiceAbstract (DapJsonCmdType::CLIENT_INFO, parent)
{
//  connect (this, &DapCmdSystemTray::interfaceRequest,
//           this, &DapCmdSystemTray::sendInterfaceRequest,
//           Qt::QueuedConnection);
}

DapCmdSystemTraySrv::~DapCmdSystemTraySrv()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdSystemTraySrv::handle (const QJsonObject *params)
{
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
