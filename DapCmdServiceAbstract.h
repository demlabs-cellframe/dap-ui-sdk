#ifndef DAPCMDSERVICEABSTRACT_H
#define DAPCMDSERVICEABSTRACT_H

#include "DapCmdAbstract.h"

class DapCmdServiceAbstract: public DapCmdAbstract
{
public:
    DapCmdServiceAbstract(DapJsonCmdType cmd, QObject* parent) :
        DapCmdAbstract(Side::SERVICE, cmd, parent) { }
    virtual ~DapCmdServiceAbstract() {}
    virtual void sendSimpleError(int code, const QString& message) final {
        QJsonObject response;
        QJsonObject errorObj;
        errorObj["code"] = code;
        errorObj["message"] = message;
        response["error"] = errorObj;
        sendCmd(&response);
    }
};

#endif // DAPCMDSERVICEABSTRACT_H
