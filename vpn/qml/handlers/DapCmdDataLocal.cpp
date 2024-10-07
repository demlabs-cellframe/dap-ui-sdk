/* INCLUDES */

#include "DapCmdDataLocal.h"

/* DEFINES */

#define DEBUGINFO qDebug() << " --->UiCMD<--- "

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdDataLocal::DapCmdDataLocal (QObject *a_parent)
  : DapCmdClientAbstract (DapJsonCmdType::DATA_LOCAL, a_parent)
{

}

DapCmdDataLocal::~DapCmdDataLocal()
{

}

/********************************************
 * OVERRIDE
 *******************************************/

void DapCmdDataLocal::handleResult (const QJsonObject &a_params)
{
  static const char *base64sign = "base64,";
  QString action  = a_params.value ("action").toString();

  /*-----------------------------------------*/

  if (action == "setAll")
  {
    QString clType    = a_params.value ("clientType").toString();

    if (!m_clientType.isEmpty()
        && clType != m_clientType)
      return;

    QJsonObject jobj  = a_params.value ("data").toObject();
    int mid           = a_params.value ("mid").toInt();
    return emit sigGotAllData (std::move (jobj), mid, std::move (clType));
  }

  /*-----------------------------------------*/

  if (action == "set")
  {
    QString name    = a_params.value ("name").toString();
    int mid         = a_params.value ("mid").toInt();

    QVariant value;
    QJsonValue jsValue  = a_params.value ("value");

    /* check if string */
    if (jsValue.type() == QJsonValue::String)
    {
      QString stringValue = jsValue.toString();

      /* check if base 64 */
      if (stringValue.startsWith (base64sign))
      {
        /* parse and store bytearray */
        auto base64src  = stringValue.mid (strlen (base64sign)).toUtf8();
        auto base64data = QByteArray::fromBase64 (base64src);
        value           = std::move (base64data);
      }

      /* store string */
      else
        value = stringValue;
    }

    /* store value */
    else
      value = jsValue.toVariant();

    //QByteArray data = QByteArray::fromBase64 (a_params.value ("value").toString().toUtf8());
    //return emit sigGotValue (std::move (name), std::move (data), mid);
    return emit sigGotValue (std::move (name), std::move (value), mid);
  }

  /*-----------------------------------------*/

  if (action == "remove")
  {
    QString name    = a_params.value ("name").toString();
    int mid         = a_params.value ("mid").toInt();
    return emit sigGotRemove (std::move (name), mid);
  }

  /*-----------------------------------------*/
}

void DapCmdDataLocal::handleError (int code, const QString &message)
{
  DEBUGINFO << __PRETTY_FUNCTION__ << QString ("%1 %2").arg (code).arg (message);
}

/********************************************
 * METHODS
 *******************************************/

const QString &DapCmdDataLocal::clientTypeName() const
{
  return m_clientType;
}

void DapCmdDataLocal::setClientTypeName (const QString &a_clientType)
{
  m_clientType = a_clientType;
}

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
    { "clientType", m_clientType },
    { "mid", a_msgId },
  };
  sendCmd (&jobj);
}

void DapCmdDataLocal::sendValue (const QString &a_name, const QVariant &a_value, const int a_msgId)
{
  QJsonObject jobj {
    { "action", "set" },
    { "name", a_name },
    { "mid", a_msgId },
    //{ "value", a_value.toBase64().constData() },
  };

  if (a_value.type() == QVariant::ByteArray)
    jobj.insert ("value", QString ("base64,") + a_value.toByteArray().toBase64().constData());
  else
    jobj.insert ("value", a_value.toJsonValue());

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
