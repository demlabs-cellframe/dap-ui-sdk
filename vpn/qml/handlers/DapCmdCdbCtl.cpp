#include "DapCmdCdbCtl.h"
#include "DapDataLocal.h"
#include "DapCdbManager.h"
#include "dap_net.h"

enum DapCmdCdbCtlValue
{
  GET,
  SET,
};

const QString DapCmdCdbCtl::s_fieldCdb = DapJsonParams::toString (DapJsonParams::CDB);
const QString DapCmdCdbCtl::s_fieldValue = DapJsonParams::toString (DapJsonParams::VALUE);

DapCmdCdbCtl::DapCmdCdbCtl (QObject *parent)
  : DapCmdClientAbstract (DapJsonCmdType::CDB_CTL, parent)
{
}

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

void DapCmdCdbCtl::_updateCmds(const QJsonValue& a_value)
{
    QString src = a_value.toString();
    QStringList list = src.split(',', Qt::SkipEmptyParts);

    auto& manager = DapCdbManager::instance();
    manager.clear();

    for (const QString& item : list)
    {
        QString host;
        uint16_t port = 0;

        QByteArray str = item.toLatin1();
        char* raw = str.data();
        char l_host[DAP_HOSTADDR_STRLEN + 1] = {'\0'};
        if (dap_net_parse_config_address(raw, l_host, &port, nullptr, nullptr)) {
            manager.addServer(QString(l_host), port ? port : 80);
        }
    }

    emit sigCdbList();
}

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
