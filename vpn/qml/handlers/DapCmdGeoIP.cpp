/* INCLUDES */

#include "DapCmdGeoIP.h"

/* DEFINES */

#define DEBUGINFO qDebug() << " --->UiCMD<--- "

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdGeoIP::DapCmdGeoIP (QObject *a_parent)
  : DapCmdClientAbstract (DapJsonCmdType::GEO_IP, a_parent)
{

}

DapCmdGeoIP::~DapCmdGeoIP()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdGeoIP::handleResult (const QJsonObject &a_params)
{
  emit sigGeoIP (a_params.value ("geo_ip").toString());
}

void DapCmdGeoIP::handleError (int code, const QString &message)
{
  DEBUGINFO << __PRETTY_FUNCTION__ << QString ("%1 %2").arg (code).arg (message);
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdGeoIP::sendRequest()
{
  sendCmd();
}

/*-----------------------------------------*/
