#include "DapCmdLastConnectionDataSrv.h"
#include <QDateTime>

/// Process command.
/// @param params Command parameters.
void DapCmdLastConnectionDataSrv::handle(const QJsonObject *params)
{
    Q_UNUSED(params);
    QJsonObject obj;
    obj["last_connection"] = mStartTime;
    sendCmd(&obj);
}

void DapCmdLastConnectionDataSrv::saveConnectionStartTime()
{
    if (!mNoReset)
        mStartTime = QDateTime::currentDateTime()
                     .toString ("hh:mm:ss dd.MM.yyyy");
}

void DapCmdLastConnectionDataSrv::dontReset(bool b) {
    mNoReset = b;
}
