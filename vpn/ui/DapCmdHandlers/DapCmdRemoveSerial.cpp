#include "DapCmdRemoveSerial.h"

DapCmdRemoveSerial::DapCmdRemoveSerial(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_REMOVE_SERIAL_REQUEST, parent)
{

}

void DapCmdRemoveSerial::sendRemoveSerialRequest(const QString &a_serial)
{
    QJsonObject obj;
    obj["serial"] = a_serial;
    sendCmd(&obj);
}

void DapCmdRemoveSerial::handleResult(const QJsonObject& result)
{
//    if (result.value("remove_serial_answer").toBool())
    emit serialRemoved();
}

void DapCmdRemoveSerial::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
    emit serialNotRemoved();
}
