#include "DapCmdRemoveSerial.h"

DapCmdRemoveSerial::DapCmdRemoveSerial(QObject *parent)
    : DapCmdServiceAbstract (DapJsonCmdType::SEND_REMOVE_SERIAL_REQUEST, parent)
{

}

DapCmdRemoveSerial::~DapCmdRemoveSerial()
{

}

void DapCmdRemoveSerial::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    connect(this, &DapCmdRemoveSerial::sendRemoveSerialAnswer, [=](const bool& removed){
        if (removed){
            QJsonObject obj;
            sendCmd(&obj);
        } else
            sendSimpleError(-45233, "Serial number is not removed");
    });

    QString serial = params->value("serial").toString();

    if (!serial.isNull()){

        emit removeSerial(serial);

    } else {
        qWarning() << "Bad value" << *params;
        return;
    }
}
