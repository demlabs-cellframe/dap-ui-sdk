#include "DapCmdResetSerialKey.h"

DapCmdResetSerialKey::DapCmdResetSerialKey(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, parent)
{
}

void DapCmdResetSerialKey::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);

    connect(this, &DapCmdResetSerialKey::sigResetSerialKeyReplied, [&] (const QString& reply) {
        QJsonObject l_obj;
        l_obj["reset_reply"] = reply;
        sendCmd(&l_obj);
    });

    emit sigResetRequestSent();
}

void DapCmdResetSerialKey::sendResetSerialError(const int& a_errorCode, const QString& error)
{
    qWarning() << "Error code: " << a_errorCode << ", message: " << error;

    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = a_errorCode;
    errorObj["message"] = error;
    response["error"] = errorObj;

    sendCmd(&response);
}

