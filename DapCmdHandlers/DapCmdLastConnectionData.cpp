#include "DapCmdLastConnectionData.h"
#include <QDateTime>

/// Process command.
/// @param params Command parameters.
void DapCmdLastConnectionData::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
    QJsonObject obj;
    obj["last_connection"] = mStartTime;
    sendCmd(&obj);
}

void DapCmdLastConnectionData::saveConnectionStartTime()
{
    mStartTime = QDateTime::currentDateTime().toString();
}
