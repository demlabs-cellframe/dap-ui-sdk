#include "DapCmdResetSerialKeySrv.h"

DapCmdResetSerialKeySrv::DapCmdResetSerialKeySrv(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, parent) {
    connect(this, &DapCmdResetSerialKeySrv::sigResetSerialKeyReplied, [&] (const QString& reply) {
        QJsonObject l_obj;
        l_obj["reset_reply"] = reply;
        sendCmd(&l_obj);
    });
}

void DapCmdResetSerialKeySrv::handle(const QJsonObject *params) {
    emit sigResetRequestSent(params->value("serial").toString());
}

void DapCmdResetSerialKeySrv::sendResetSerialError(const int& a_errorCode, const QString& error)
{
    qWarning() <<"Error message: "<< error;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = a_errorCode;
    errorObj["message"] = error;
    response["error"] = errorObj;

    sendCmd(&response);
}
