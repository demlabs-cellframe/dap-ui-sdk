/* INCLUDES */

#include "DapCmdDataLocal.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdDataLocal::DapCmdDataLocal (QObject *a_parent)
  : DapCmdServiceAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{

}

DapCmdDataLocal::~DapCmdDataLocal()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdDataLocal::handle (const QJsonObject *a_params)
{
  QString action  = a_params->value ("action").toString();

  /*-----------------------------------------*/

  if (action == "setAll")
  {
    QJsonObject jobj  = a_params->value ("data").toObject();
    int mid           = a_params->value ("mid").toInt();
    return emit sigGotAllData (std::move (jobj), mid);
  }

  /*-----------------------------------------*/

  if (action == "set")
  {
    QString name    = a_params->value ("name").toString();
    int mid         = a_params->value ("mid").toInt();
    QByteArray data = QByteArray::fromBase64 (a_params->value ("value").toString().toUtf8());
    return emit sigGotValue (std::move (name), std::move (data), mid);
  }

  /*-----------------------------------------*/

  if (action == "remove")
  {
    QString name    = a_params->value ("name").toString();
    int mid         = a_params->value ("mid").toInt();
    return emit sigGotRemove (std::move (name), mid);
  }

  /*-----------------------------------------*/
}

/********************************************
 * METHODS
 *******************************************/

void DapCmdDataLocal::requestValue (const QString &a_name, const int a_msgId)
{
  QJsonObject jobj {
    { "action", "get" },
    { "name", a_name },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocal::requestAllData (const int a_msgId)
{
  QJsonObject jobj {
    { "action", "getAll" },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocal::sendValue (const QString &a_name, const QByteArray &a_value, const int a_msgId)
{
  QJsonObject jobj {
    { "action", "set" },
    { "name", a_name },
    { "mid", a_msgId },
    { "value", a_value.toBase64().constData() },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocal::sendRemove (const QString &a_name, const int a_msgId)
{
  QJsonObject jobj {
    { "action", "remove" },
    { "name", a_name },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

/*-----------------------------------------*/
