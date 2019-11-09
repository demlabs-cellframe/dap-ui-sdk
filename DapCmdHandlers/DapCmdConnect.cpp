#include "DapCmdConnect.h"

DapCmdConnect::DapCmdConnect(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::CONNECTION, parent)
{

}

void DapCmdConnect::sendCmdConnect(const QString& a_addr, quint16 a_port,
                                   const QString& a_user, const QString& a_pswd)
{
    QJsonObject obj;
    obj["action"] = "Connect";
    obj["address"] = a_addr;

    if ( !a_user.isEmpty() )
        obj["user"] = a_user;
    if ( !a_pswd.isEmpty())
        obj["password"] = a_pswd;
    obj["port"] = a_port;
    sendCmd(&obj);
}

void DapCmdConnect::sendCmdDisconnect()
{
    QJsonObject obj;
    obj["action"] = "Disconnect";
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
    Q_UNUSED(code);
    qWarning() << *m_errorObject;
    emit errorMessage(message);
}
