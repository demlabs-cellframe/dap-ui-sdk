#include "DapCmdSignUpSrv.h"

DapCmdSignUpSrv::DapCmdSignUpSrv(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_SIGN_UP_REQUEST, parent)
{
    connect(this, &DapCmdSignUpSrv::sigSendCmdSignUpAnswer, [=](const QString& answer){
        QJsonObject obj;
        obj["sign_up_answer"] = answer;
        sendCmd(&obj);
    });
}

DapCmdSignUpSrv::~DapCmdSignUpSrv()
{

}


void DapCmdSignUpSrv::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

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
