#include "DapCmdResetSerial.h"
#include <QJsonArray>

DapCmdResetSerial::DapCmdResetSerial(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::SEND_RESET_SERIAL_REQUEST, parent)
{

}

void DapCmdResetSerial::sendCmdResetSerialRequest(const QString &serial)
{
    QJsonObject obj;
    obj["serial"] = serial;
    sendCmd(&obj);
}

void DapCmdResetSerial::handleResult(const QJsonObject& result)
{
    qDebug() << "handleResult";
    if(!result.contains("reset_serial_answer")) {
        qWarning() << "Not found mandatory parameter!";
        return;
    }
    emit resetSerialAnswer(result.value("reset_serial_answer").toString());
}

void DapCmdResetSerial::handleError(int code, const QString& message)
{
    qDebug() << "handleError";
    Q_UNUSED(code);
    qWarning() << *m_errorObject;
    emit resetSerialAnswerIsError(message);
}
