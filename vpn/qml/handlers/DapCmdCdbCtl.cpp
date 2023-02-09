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
  : DapCmdClientAbstract (DapJsonCmdType::CDB_CTL, parent)
{

}

/********************************************
 * METHODS
 *******************************************/

void DapCmdCdbCtl::sendCmdSetList (const QString &a_value)
{
  QJsonObject obj =
  {
    {s_fieldCdb,    a_value}, // example: "127.0.0.1,127.0.0.2,127.0.0.3"
    {s_fieldValue,  DapCmdCdbCtlValue::SET},
  };
  sendCmd(&obj);

  _updateCmds (obj.value (s_fieldCdb));
}

void DapCmdCdbCtl::sendCmdGetList()
{
  QJsonObject obj =
  {
    {s_fieldCdb,    QString()},
    {s_fieldValue,  DapCmdCdbCtlValue::GET},
  };
  sendCmd(&obj);
}

void DapCmdCdbCtl::_updateCmds (const QJsonValue &a_value)
{
  /* get actual list and update */
  auto src  = a_value.toString();
  auto list = src.split(',');
  auto cdbs = DapCdbServerList::toServers (list);

  auto data = DapDataLocal::instance();
  data->updateCdbList (cdbs);

  emit sigCdbList();
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdCdbCtl::handleResult (const QJsonObject &a_result)
{
  /* get values */
  auto cdb    = a_result.value (s_fieldCdb);
  auto value  = a_result.value (s_fieldValue);

  /* check if valid */
  if (cdb == QJsonValue::Undefined
      || value == QJsonValue::Undefined)
    {
      qWarning() << a_result;
      return;
    }

  /* handle message */
  if (value.toInt() == DapCmdCdbCtlValue::GET)
    _updateCmds (cdb);
  else
    qWarning() << "Service should not send SET value." << a_result;
}

void DapCmdCdbCtl::handleError (int, const QString &)
{
  qWarning() << *m_errorObject;
}

/*-----------------------------------------*/
