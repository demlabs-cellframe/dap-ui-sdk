#include "DapCmdResetSerialKey.h"

void DapCmdResetSerialKey::handle(const QJsonObject *params) {
    connect(this, &DapCmdResetSerialKey::sigResetSerialKeyReplied, [&] (const QString& reply) {
        QJsonObject l_obj;
        l_obj["reset_reply"] = reply;
        sendCmd(&l_obj);
    });
    emit sigResetRequestSent(params->value("serial").toString());
}

void DapCmdResetSerialKey::sendResetSerialError(const int& a_errorCode, const QString& error)
{
    qWarning() <<"Error message: "<< error;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = a_errorCode;
    errorObj["message"] = error;
    response["error"] = errorObj;

    sendCmd(&response);
}
