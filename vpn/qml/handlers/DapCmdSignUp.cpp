#include "DapCmdSignUp.h"

#include <QTimer>

DapCmdSignUp::DapCmdSignUp(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_SIGN_UP_REQUEST, parent)
{

}

void DapCmdSignUp::sendSignUpRequest(const QString &a_site, const QString &a_email, const QString &a_password)
{
    QJsonObject obj;
    obj["site"] = a_site;
    obj["email"] = a_email;
    obj["password"] = a_password;
    sendCmd(&obj);
}

void DapCmdSignUp::handleResult(const QJsonObject& result)
{
    QString request = result.value("sign_up_answer").toString();
    qDebug() << request;
    emit sigSignUpAnswer(request);
}

void DapCmdSignUp::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}
