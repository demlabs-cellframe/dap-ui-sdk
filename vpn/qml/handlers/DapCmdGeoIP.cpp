/* INCLUDES */

#include "DapCmdGeoIP.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdGeoIP::DapCmdGeoIP (QObject *a_parent)
  : DapCmdServiceAbstract (DapJsonCmdType::GEO_IP, a_parent)
{

}

DapCmdGeoIP::~DapCmdGeoIP()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdGeoIP::handle (const QJsonObject *a_params)
{
  if (a_params)
    emit sigGeoIP (a_params->value ("geo_ip").toString());
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdGeoIP::sendRequest()
{
  sendCmd();
}

/*-----------------------------------------*/
