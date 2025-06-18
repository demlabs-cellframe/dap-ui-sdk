/* INCLUDES */

#include "DapCmdGeoIP.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdGeoIP::DapCmdGeoIP (QObject *a_parent)
    : DapCmdServiceAbstract (DapJsonCmdType::GEO_IP, a_parent)
{
    connect (this, &DapCmdGeoIP::sigSendGeoIP,
            [this] (const QString &a_value)
            {
                QJsonObject jobj{{ "geo_ip", a_value }};
                sendCmd (&jobj);
            });
}

DapCmdGeoIP::~DapCmdGeoIP()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdGeoIP::handle (const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    emit sigGetGeoIP();
}

/*-----------------------------------------*/
