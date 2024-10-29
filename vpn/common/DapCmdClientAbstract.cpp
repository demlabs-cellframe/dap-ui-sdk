#include "DapCmdClientAbstract.h"

DapCmdClientAbstract::~DapCmdClientAbstract()
{

}


void DapCmdClientAbstract::handle(const QJsonObject* params)
{
    if(params->contains("error")) {
        auto errorVal = params->value("error");
        if(!errorVal.toObject().contains("code") ||
                !errorVal.toObject().contains("message")) {
            qCritical() << "Bad error response from service. Must contains: code and message";
            return;
        }
        int code = errorVal.toObject()["code"].toInt();
        QString message = errorVal.toObject()["message"].toString();
        m_errorObject = params;
        handleError(code, message);
        m_errorObject = Q_NULLPTR;
        return;
    }

    handleResult(*params);
}
