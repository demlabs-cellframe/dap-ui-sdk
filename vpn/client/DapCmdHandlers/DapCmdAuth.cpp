#include "DapCmdAuth.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdAuth::actionParam = "action";

/**
 * @brief DapCmdAuth::DapCmdAuth
 * @param parent
 */
DapCmdAuth::DapCmdAuth(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::AUTHORIZE, parent) {

}

/**
 * @brief DapCmdAuth::~DapCmdAuth
 */
DapCmdAuth::~DapCmdAuth()
{

}

/**
 * @brief DapCmdAuth::sendCmdErrorAuth
 * @param errorMsg
 */
void DapCmdAuth::sendCmdErrorAuth(const QString errorMsg)
{
    qWarning() << errorMsg;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = -31000; // TODO! This is random digit
    errorObj["message"] = errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

void DapCmdAuth::sendCmdErrorAuth(const int& a_errorCode, const QString a_errorMsg)
{
    qWarning() <<"Error message: "<< a_errorMsg;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = a_errorCode;
    errorObj["message"] = a_errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

/**
 * @brief DapCmdAuth::handle
 * @param params
 */
void DapCmdAuth::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    if(params->value(actionParam).toString() == "Auth" ){

        QMap<QString, QJsonValue> mandatoryConnParams = {
            {"address", QJsonValue::Undefined },
            {"password", QJsonValue::Undefined },
            {"port", QJsonValue::Undefined },
            {"user", QJsonValue::Undefined },
        };

        for(auto &k: mandatoryConnParams.keys()) {
            QJsonValue val = params->value(k);
            if (val == QJsonValue::Undefined) {
                qWarning() << "Not found mandatory key"
                           << k << " for establish a connection";
                return;
            }
            mandatoryConnParams[k] = val;
        }

        emit sigAuth(mandatoryConnParams["user"].toString(),
                mandatoryConnParams["password"].toString(),
                mandatoryConnParams["address"].toString(),
                uint16_t(mandatoryConnParams["port"].toInt()));
    }
}
