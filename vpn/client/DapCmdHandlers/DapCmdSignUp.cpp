#include "DapCmdSignUp.h"

DapCmdSignUp::DapCmdSignUp(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_SIGN_UP_REQUEST, parent)
{

}

DapCmdSignUp::~DapCmdSignUp()
{

}


void DapCmdSignUp::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    connect(this, &DapCmdSignUp::sigSendCmdSignUpAnswer, [=](const QString& answer){
        QJsonObject obj;
        obj["sign_up_answer"] = answer;
        sendCmd(&obj);
    });

    QString site = params->value("site").toString();
    QString email = params->value("email").toString();
    QString password = params->value("password").toString();

    if (!email.isNull() && !password.isNull()){

        emit sigSignUpRequest(site, email, password);

    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
