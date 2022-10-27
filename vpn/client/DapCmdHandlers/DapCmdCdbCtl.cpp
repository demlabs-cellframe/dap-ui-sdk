/* INCLUDES */
#include "DapCmdCdbCtl.h"
#include "DapDataLocal.h"

/* DEFS */

enum DapCmdCdbCtlValue
{
  GET,
  SET,
};

/* VARS */

const QString DapCmdCdbCtl::s_fieldCdb =
  DapJsonParams::toString (DapJsonParams::CDB);

const QString DapCmdCdbCtl::s_fieldValue =
  DapJsonParams::toString (DapJsonParams::VALUE);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdCdbCtl::DapCmdCdbCtl (QObject *parent)
  : DapCmdServiceAbstract (DapJsonCmdType::CDB_CTL, parent)
{

}

/********************************************
 * METHODS
 *******************************************/

void DapCmdCdbCtl::sendCmdGetList (const QString &a_value)
{
  static QJsonObject result = { {s_fieldValue, DapCmdCdbCtlValue::GET} };
  result[s_fieldCdb]        = a_value;
  sendCmd(&result);
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdCdbCtl::handle (const QJsonObject *a_params)
{
  /* get value and check */
  auto value  = a_params->value (s_fieldValue);
  if (value == QJsonValue::Undefined)
    {
      qWarning() << "Invalid value" << a_params;
      return;
    }

  /* behave based on value */
  switch (DapCmdCdbCtlValue (value.toInt()))
    {

    /* --------------------------- */
    /* REQUESTED CURREENT CDB LIST */
    /* --------------------------- */

    case DapCmdCdbCtlValue::GET:
      {
        /* collect servers into string list */
        const auto &list  = DapDataLocal::instance()->cdbServersList();
        QStringList result;

        for (const auto &item : list)
          result << item;

        /* join and send */
        sendCmdGetList (result.join (','));
      } break;

    /* ------------------------ */
    /* SET NEW CDB SERVERS LIST */
    /* ------------------------ */

    case DapCmdCdbCtlValue::SET:
      {
        static const QString SETTING_CDB { "cdb" };

        /* get new cdb servers list and check */
        auto cdb  = a_params->value (s_fieldCdb);
        if (value == QJsonValue::Undefined)
          {
            qWarning() << "Invalid cdb list" << a_params;
            return;
          }

        /* get actual list and update */
        auto cdbs = cdb.toString();
        auto list = cdbs.split(',');

        auto data = DapDataLocal::instance();
        data->updateCdbList (list);
        data->saveSetting (SETTING_CDB, cdbs.toLatin1().toBase64());
      } break;

    };
}

/*-----------------------------------------*/
