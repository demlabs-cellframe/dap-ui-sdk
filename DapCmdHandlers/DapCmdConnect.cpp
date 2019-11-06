#include "DapCmdConnect.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdConnect::actionParam = "action";

DapCmdConnect::DapCmdConnect(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::CONNECTION, parent) {

}

DapCmdConnect::~DapCmdConnect()
{

}

void DapCmdConnect::sendCmdErrorAuth(const QString errorMsg)
{
    qWarning() << errorMsg;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = -31000; // TODO! This is random digit
    errorObj["message"] = errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

void DapCmdConnect::handle(const QJsonObject* params) {
    if(params->value(actionParam) != QJsonValue::Undefined) { //this is disconnect request
        if(params->value(actionParam).toString() == "Disconnect") {
            emit sigDisconnect();
            return;
        }
    }

    QMap<QString, QJsonValue> mandatoryConnParams = {
        {"address", QJsonValue::Undefined },
        {"password", QJsonValue::Undefined },
        {"port", QJsonValue::Undefined },
        {"user", QJsonValue::Undefined },
    };

    for(auto &k: mandatoryConnParams.keys()) {
        QJsonValue val = params->value(k);
        if (val != QJsonValue::Undefined) {
            mandatoryConnParams[k] = val;
        }
    }
    
    if ( mandatoryConnParams["port"] != QJsonValue::Undefined  && mandatoryConnParams["address"] != QJsonValue::Undefined ){
        if ( mandatoryConnParams["address"] != QJsonValue::Undefined  && mandatoryConnParams["password"] != QJsonValue::Undefined )
            emit sigConnect(mandatoryConnParams["user"].toString(),
                            mandatoryConnParams["password"].toString(),
                            mandatoryConnParams["address"].toString(),
                            uint16_t(mandatoryConnParams["port"].toInt()));
        else
            emit sigConnectNoAuth( mandatoryConnParams["address"].toString(),
                             uint16_t(mandatoryConnParams["port"].toInt()));

    }
}
