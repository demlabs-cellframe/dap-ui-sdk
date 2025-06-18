#include "DapCmdLastConnectionData.h"
#include <QDateTime>

/// Process command.
/// @param params Command parameters.
void DapCmdLastConnectionData::handle(const QJsonObject *params)
{
    DapCmdServiceAbstract::handle(params);
    QJsonObject obj;
    obj["last_connection"] = mStartTime;
    sendCmd(&obj);
}

void DapCmdLastConnectionData::saveConnectionStartTime()
{
    if (!mNoReset)
        mStartTime = QDateTime::currentDateTime()
                     .toString ("hh:mm:ss dd.MM.yyyy");
}

void DapCmdLastConnectionData::dontReset(bool b) {
    mNoReset = b;
}
