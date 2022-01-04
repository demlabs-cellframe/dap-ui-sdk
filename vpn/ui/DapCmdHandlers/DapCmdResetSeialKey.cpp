#include "DapCmdResetSeialKey.h"

DapCmdResetSeialKey::DapCmdResetSeialKey(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, parent)
{

}

void DapCmdResetSeialKey::sendResetSerialKeyRequest(const QString &a_serial /*= ""*/)
{
    if (a_serial.isEmpty()){
        qDebug() << "Serial is empty";
        return;
    }
    QJsonObject obj;
    obj["serial"] = a_serial;
    sendCmd(&obj);
}

void DapCmdResetSeialKey::handleResult(const QJsonObject& result)
{
    QString request = result.value("reset_reply").toString();
    qDebug() << "Serial reset answer: " << request;
    if (!request.contains("successfully", Qt::CaseInsensitive))
        emit sigResetSerialKeyError(request.remove(QRegExp("\\D")));
    else emit sigSerialKeyReseted();
}

void DapCmdResetSeialKey::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}

