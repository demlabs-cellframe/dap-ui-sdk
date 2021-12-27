#include "DapCmdResetSerialKey.h"

void DapCmdResetSerialKey::handle(const QJsonObject *params) {
    connect(this, &DapCmdResetSerialKey::sigResetRequestReplied, [&] (const QString& reply) {
        QJsonObject l_obj;
        l_obj["reset_reply"] = reply;
        sendCmd(&l_obj);
    });
    emit sigResetRequestSent(params->value("serial").toString());
}
