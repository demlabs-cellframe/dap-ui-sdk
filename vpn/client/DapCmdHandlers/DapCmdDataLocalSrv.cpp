/* INCLUDES */

#include "DapCmdDataLocalSrv.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdDataLocalSrv::DapCmdDataLocalSrv (QObject *a_parent)
  : DapCmdServiceAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{

}

DapCmdDataLocalSrv::~DapCmdDataLocalSrv()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdDataLocalSrv::handle (const QJsonObject *a_params)
{
  QString action  = a_params->value ("action").toString();

  /*-----------------------------------------*/

  if (action == "set")
  {
    QString name    = a_params->value ("name").toString();
    int mid         = a_params->value ("mid").toInt();
    QByteArray data = QByteArray::fromBase64 (a_params->value ("value").toString().toUtf8());

    return emit sigSetValue (std::move (name), std::move (data), mid);
  }

  /*-----------------------------------------*/

  if (action == "get")
  {
    QString name    = a_params->value ("name").toString();
    int mid         = a_params->value ("mid").toInt();
    return emit sigGetValue (std::move (name), mid);
  }

  /*-----------------------------------------*/

  if (action == "remove")
  {
    QString name    = a_params->value ("name").toString();
    int mid         = a_params->value ("mid").toInt();
    return emit sigRemoveValue (std::move (name), mid);
  }

  /*-----------------------------------------*/

  if (action == "getAll")
  {
    QString clType  = a_params->value ("clientType").toString();
    int mid         = a_params->value ("mid").toInt();
    return emit sigGetAllData (mid, std::move (clType));
  }

  /*-----------------------------------------*/
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdDataLocalSrv::sendAllData (const QJsonObject &a_data, const int a_msgId, const QString &a_clientType)
{
  QJsonObject jobj {
    { "action", "setAll" },
    { "clientType", a_clientType },
    { "data", a_data },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendRemove (const QString &a_name, const int a_msgId)
{
  QJsonObject jobj {
    { "action", "remove" },
    { "name", a_name },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocalSrv::sendValue (const QString &a_name, const QByteArray &a_data, const int a_msgId)
{
  QJsonObject jobj
  {
    { "action", "set" },
    { "name", a_name },
    { "mid", a_msgId },
    { "value", a_data.toBase64().constData() },
  };
  sendCmd (&jobj);
}

/*-----------------------------------------*/
