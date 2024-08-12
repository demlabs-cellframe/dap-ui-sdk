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

/**
 * @brief DapCmdConnect::sendCmdError
 * @param a_errorCode
 * @param a_errorMsg
 */
void DapCmdConnect::sendCmdError(int a_errorCode, const QString a_errorMsg)
{
    qWarning() << "Error message: " << a_errorMsg << " | code: " << QString::number(a_errorCode);
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = a_errorCode;
    errorObj["message"] = a_errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

/**
 * @brief DapCmdConnect::sendCmdError
 * @param errorMsg
 */
void DapCmdConnect::sendCmdError(const QString errorMsg)
{
    qWarning() << "Error message: " << errorMsg << " | code: none";
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = -31000; // TODO! This is random digit
    errorObj["message"] = errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

void DapCmdConnect::handle(const QJsonObject* params) {
    if(params->value(actionParam) != QJsonValue::Undefined) 
    { 
        //this is disconnect request
        QString req = params->value(actionParam).toString();
        if(req == "Disconnect") 
        {
            qDebug() << "DapCmdConnect::Disconnect signal";
            emit sigDisconnect();
            return;
        } 
        
        if (req == "RestartService")
        {
            qDebug() << "DapCmdConnect::RestartService signal";
            emit sigRestartService(false);
            return;
        }

        if (req == "RestartServiceIfRunning")
        {
            qDebug() << "DapCmdConnect::RestartServiceIfRunning signal";
            emit sigRestartService(true);
            return;
        }
    }

    QMap<QString, QJsonValue> mandatoryConnParams = {
        {"address", QJsonValue::Undefined },
        {"password", QJsonValue::Undefined },
        {"port", QJsonValue::Undefined },
        {"user", QJsonValue::Undefined },
        {"serial", QJsonValue::Undefined }
    };

    for(auto &k: mandatoryConnParams.keys()) {
        QJsonValue val = params->value(k);
        if (val != QJsonValue::Undefined) {
            mandatoryConnParams[k] = val;
        }
    }

    if ( mandatoryConnParams["port"] != QJsonValue::Undefined  && mandatoryConnParams["address"] != QJsonValue::Undefined ){
        if ( mandatoryConnParams["address"] != QJsonValue::Undefined /* && mandatoryConnParams["password"] != QJsonValue::Undefined */)

                emit sigConnect(mandatoryConnParams["serial"].toString(),
                                mandatoryConnParams["user"].toString(),
                                mandatoryConnParams["password"].toString(),
                                mandatoryConnParams["address"].toString(),
                                uint16_t(mandatoryConnParams["port"].toInt()));

        else
            emit sigConnectNoAuth( mandatoryConnParams["address"].toString(),
                             uint16_t(mandatoryConnParams["port"].toInt()));

    }
}
