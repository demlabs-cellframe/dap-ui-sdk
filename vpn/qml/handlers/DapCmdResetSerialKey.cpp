#include "DapCmdResetSerialKey.h"

DapCmdResetSerialKey::DapCmdResetSerialKey(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, parent)
{

}

void DapCmdResetSerialKey::sendResetSerialKeyRequest(const QString &a_serial /*= ""*/)
{
    QJsonObject obj;
    obj["serial"] = a_serial;
    sendCmd(&obj);
}

void DapCmdResetSerialKey::handleResult(const QJsonObject& result)
{
    QString request = result.value("reset_reply").toString();
    qDebug() << "Serial reset answer: " << request;
    emit sigSerialKeyReseted();
}

void DapCmdResetSerialKey::handleError(int code, const QString& message)
{
    qWarning() << "Error code:" << code << ", message:" << message;
    qWarning() << *m_errorObject;

    switch (code) {
    case 1:
        emit sigResetSerialKeyError(message);
        break;
    case 2:
        emit sigResetSerialKeyErrorSetOnlyMessage(message);
        break;
    default:
        qWarning() << "Unknown error code received.";
        break;
    }
}

