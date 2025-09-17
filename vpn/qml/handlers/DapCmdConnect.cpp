#include "DapCmdConnect.h"

DapCmdConnect::DapCmdConnect(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::CONNECTION, parent)
{

}

void DapCmdConnect::sendCmdConnect(const QString& a_addr, quint16 a_port,
                                   const QString& a_user, const QString& a_pswd,  const QString& a_serial, const bool a_updateRouteTable,
                                   const QString& a_runScriptPath, const bool a_runAfterConnect)
{
    QJsonObject obj;
    obj["action"] = "Connect";
    obj["address"] = a_addr;
    obj["port"] = a_port;
    obj["updateRouteTable"] = a_updateRouteTable;
    if ( !a_user.isEmpty() )
        obj["user"] = a_user;
    if ( !a_pswd.isEmpty())
        obj["password"] = a_pswd;
    if ( !a_serial.isEmpty() )
        obj["serial"] = QString(a_serial).remove('-');
    if ( !a_runScriptPath.isEmpty() ) {
        obj["run_script_path"] = QString(a_runScriptPath);
        obj["run_script_after_connect"] = a_runAfterConnect;
    }

    sendCmd(&obj);
}

void DapCmdConnect::sendCmdConnectByOrder(const QString& a_addr)
{
    QJsonObject obj;
    obj["action"] = "ConnectByOrder";
    obj["address"] = a_addr;

    obj["port"] = 80;
    sendCmd(&obj);
}

void DapCmdConnect::sendCmdDisconnect()
{
    QJsonObject obj;
    obj["action"] = "Disconnect";
    sendCmd(&obj);
}

void DapCmdConnect::sendCmdRestartService(bool if_running)
{
    QJsonObject obj;
    if (if_running)
        obj["action"] = "RestartServiceIfRunning";
    else
        obj["action"] = "RestartService";
    sendCmd(&obj);
}

void DapCmdConnect::handleResult(const QJsonObject& result)
{
    qDebug() << "handleResult";
    if(!result.contains("message")) {
        qWarning() << "Not found mandatory parameter!";
        return;
    }
    emit errorMessage(result.value("message").toString());
}

void DapCmdConnect::handleError(int code, const QString& message)
{
    qDebug() << "handleError";
//    Q_UNUSED(code);
    qWarning() << *m_errorObject;
    // Code: -10 (the hash of the conditional transaction) is temporarily ignored.
    if (code == -10)
    {
        qWarning() << "Code: -10 (the hash of the conditional transaction) is temporarily ignored.";
        return;
    }
    if (code == 6543 || code == -1025){
        emit errorMessage("Server not available. Please, try others or the same later");
        return;
    }
    // sevice connection line fail
    if (code == 110)
    {
        emit connectionFail();
        emit errorMessage("Server not available. Network connection is disabled.");
        return;
    }
  // network is unreachable
  if (code == 101)
  {
      emit errorMessage("Server not available. Network connection is disabled.");
      return;
  }
    // unknown error
    if (code == 10053 && message == "Unknown error")
        return;
    emit errorMessage(message);
    QString textError = QString("code: %1, message: %2").arg(QString::number(code)).arg(message);
    emit errorMessageWithCode(textError);
}
