#include "DapCmdAuth.h"

DapCmdAuth::DapCmdAuth(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::CONNECTION, parent)
{

}

/**
 * @brief DapCmdAuth::sendCmdAuth
 * @param addr
 * @param port
 * @param user
 * @param pswd
 */
void DapCmdAuth::sendCmdAuth(const QString &addr, quint16 port, const QString &user, const QString &pswd)
{
    QJsonObject obj;
    obj["action"] = "Auth";
    obj["address"] = addr;
    obj["password"] = pswd;
    obj["port"] = port;
    obj["user"] = user;
    sendCmd(&obj);
}

void DapCmdAuth::handleResult(const QJsonObject& result)
{
    qDebug() << "handleResult";
    if(!result.contains("message")) {
        qWarning() << "Not found mandatory parameter!";
        return;
    }
    emit errorMessage(result.value("message").toString());
}

void DapCmdAuth::handleError(int code, const QString& message)
{
    qDebug() << "handleError";
    Q_UNUSED(code);
    qWarning() << *m_errorObject;
    emit errorMessage(message);
}
