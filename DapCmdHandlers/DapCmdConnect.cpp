#include "DapCmdConnect.h"

DapCmdConnect::DapCmdConnect(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::CONNECTION, parent)
{

}

void DapCmdConnect::sendCmdConnect(const QString& addr, quint16 port,
                                   const QString& user, const QString& pswd)
{
    QJsonObject obj;
    obj["action"] = "Connect";
    obj["address"] = addr;

    if ( !user.isEmpty() )
        obj["user"] = user;
    if ( !pswd.isEmpty())
        obj["password"] = pswd;
    obj["port"] = port;
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
