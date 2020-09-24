#include "DapCmdResetSerial.h"
#include "DapJsonParams.h"
#include <QJsonArray>

DapCmdResetSerial::DapCmdResetSerial(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::SEND_RESET_SERIAL_REQUEST, parent)
{

}

void DapCmdResetSerial::handle(const QJsonObject* params)
{
    Q_UNUSED(params);

    connect(this, &DapCmdResetSerial::sigResetSerialAnswer, [=](const QJsonDocument& answer){

        auto arr = answer.array();
        if (arr.isEmpty()) {
            sendSimpleError(-4546, "Empty reset serial answer");
        } else {
            QJsonObject obj;
            obj["reset_serial_answer"] = arr;
            sendCmd(&obj);
        }
    });

    QString serial = params->value("serial").toString();
    if (serial.isEmpty()){
        qDebug() << "Error reset serial request sending, serial is empty";
        return;
    }
    emit resetSerialRequest(serial);
}
