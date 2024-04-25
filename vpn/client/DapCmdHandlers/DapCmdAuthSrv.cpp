#include "DapCmdAuthSrv.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

const QString DapCmdAuthSrv::actionParam = "action";

/**
 * @brief DapCmdAuthSrv::DapCmdAuthSrv
 * @param parent
 */
DapCmdAuthSrv::DapCmdAuthSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::AUTHORIZE, parent) {

}

/**
 * @brief DapCmdAuthSrv::~DapCmdAuthSrv
 */
DapCmdAuthSrv::~DapCmdAuthSrv()
{

}

/**
 * @brief DapCmdAuthSrv::sendCmdErrorAuth
 * @param errorMsg
 */
void DapCmdAuthSrv::sendCmdErrorAuth(const QString errorMsg)
{
    qWarning() << errorMsg;
    QJsonObject response;
    QJsonObject errorObj;

    errorObj["code"] = -31000; // TODO! This is random digit
    errorObj["message"] = errorMsg;
    response["error"] = errorObj;

    sendCmd(&response);
}

void DapCmdAuthSrv::sendCmdErrorAuth(const int& a_errorCode, const QString a_errorMsg)
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
 * @brief DapCmdAuthSrv::handle
 * @param params
 */
void DapCmdAuthSrv::handle(const QJsonObject* params)
{

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
