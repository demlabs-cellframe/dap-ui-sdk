#include "DapCmdLastConnectionData.h"
#include <utility>

/// Connection start time.
const QString DapCmdLastConnectionData::startTime =
        DapJsonParams::toString(DapJsonParams::LAST_CONNECTION);

void DapCmdLastConnectionData::handleResult(const QJsonObject& result)
{
    if (result.value(startTime) != QJsonValue::Undefined) {
        DapCmdLastConnectionData::lastConnectionDataHandler(result.value(startTime));
    } else {
        qWarning() << result;
    }
}

void DapCmdLastConnectionData::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}

/// Process and verify command parameters.
/// @param startTime Connection start time.
void DapCmdLastConnectionData::lastConnectionDataHandler(const QJsonValue &startTime)
{
    QString bStartTime = startTime.toString();
    if (bStartTime.isNull() || bStartTime.isEmpty()) {
        qWarning() << "Something wrong can't parse last connection data";
        return;
    }
    QDateTime qdtStartTime  = QDateTime::fromString (bStartTime, "hh:mm:ss dd.MM.yyyy");
    qDebug() << "DapCmdLastConnectionData::lastConnectionDataHandler" << bStartTime << startTime << qdtStartTime;
    emit sigLastConnectionData (qdtStartTime);
}

void DapCmdLastConnectionData::sendCmd()
{
    DapCmdClientAbstract::sendCmd(nullptr);
}
