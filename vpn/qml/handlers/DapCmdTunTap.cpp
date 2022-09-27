#include "DapCmdTunTap.h"

void DapCmdTunTap::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}

void DapCmdTunTap::sendCmd(Action act)
{
    QString strAct = act == Action::CHECK ? "check" : "install";
    QJsonObject obj;
    obj["action"] = strAct;
    DapCmdClientAbstract::sendCmd(&obj);
}

void DapCmdTunTap::handleResult(const QJsonObject& result) {
    if (result.value("value").toString() == "ok") {
        emit sigTapOk();
    } else if (result.value("value").toString() == "error") {
        emit sigTapError();
    }
}
