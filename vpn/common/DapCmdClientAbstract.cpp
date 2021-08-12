/* INCLUDES */
#include "DapCmdClientAbstract.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapCmdClientAbstract::DapCmdClientAbstract (DapJsonCmdType cmd, QObject *parent):
  DapCmdAbstract (Side::CLIENT, cmd, parent)
{

}

DapCmdClientAbstract::~DapCmdClientAbstract()
{

}

/********************************************
 * OVERRIDES
 *******************************************/

void DapCmdClientAbstract::handle (const QJsonObject *params)
{
  /* handle result, if no error */
  if (!params->contains ("error"))
    return handleResult (*params);

  /* get error object and check fields presence */
  auto errorVal = params->value ("error").toObject();
  if (!errorVal.contains ("code")
      || !errorVal.contains ("message"))
    {
      qCritical() << "Bad error response from service. Must contains: code and message";
      return;
    }

  /* get fields and handle error */
  int code        = errorVal.value ("code").toInt();
  QString message = errorVal.value ("message").toString();
  m_errorObject   = params;

  handleError (code, message);

  m_errorObject   = Q_NULLPTR;
  return;
}

void DapCmdClientAbstract::sendCmd (const QJsonObject *obj)
{
//  if (obj == Q_NULLPTR)
//    {
//      qDebug() << "Send command" << commandToString (cmd());
//    }
//  else
//    {
//      qDebug() << "Send command" << commandToString (cmd())
//               << "params:"      << *obj;
//    }

  DapCmdAbstract::sendCmd (obj);
}
