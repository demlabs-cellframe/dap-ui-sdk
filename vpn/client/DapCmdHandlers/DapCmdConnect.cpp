#include "DapCmdConnect.h"
#include <QMap>
#include <QDebug>
#include "DapSession.h"

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

    errorObj[CODE_KEY] = a_errorCode;
    errorObj[MESSAGE_KEY] = a_errorMsg;
    response[ERROR_KEY] = errorObj;

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

    errorObj[CODE_KEY] = -31000; // TODO! This is random digit
    errorObj[MESSAGE_KEY] = errorMsg;
    response[ERROR_KEY] = errorObj;

    sendCmd(&response);
}

void DapCmdConnect::handle(const QJsonObject* params) 
{
    if(params->value(ACTION_KEY) != QJsonValue::Undefined)
    { 
        //this is disconnect request
        QString req = params->value(ACTION_KEY).toString();
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
        {ADDRESS_KEY, QJsonValue::Undefined },
        {PASSWORD_KEY, QJsonValue::Undefined },
        {PORT_KEY, QJsonValue::Undefined },
        {USER_KEY, QJsonValue::Undefined },
        {SERIAL_KEY, QJsonValue::Undefined },
        {UPDATE_ROUTE_TABLE, QJsonValue::Undefined }
    };

    for(auto &key: mandatoryConnParams.keys())
    {
        QJsonValue val = params->value(key);
        if (val != QJsonValue::Undefined) {
            mandatoryConnParams[key] = val;
        }
    }

    if ( mandatoryConnParams[PORT_KEY] != QJsonValue::Undefined  && mandatoryConnParams[ADDRESS_KEY] != QJsonValue::Undefined )
    {
        if ( mandatoryConnParams[ADDRESS_KEY] != QJsonValue::Undefined)
        {
            auto getValidString = [](const QJsonValue& value) -> QString
            {
                if(value == QJsonValue::Undefined)
                {
                    return QString();
                }
                return value.toString();
            };

            QString serialKey = getValidString(mandatoryConnParams[SERIAL_KEY]);
            QString userName = getValidString(mandatoryConnParams[USER_KEY]);
            QString password = getValidString(mandatoryConnParams[PASSWORD_KEY]);
            bool updateRouteTable = mandatoryConnParams[UPDATE_ROUTE_TABLE].toBool(true);

            if(serialKey.isEmpty() && userName.isEmpty() && password.isEmpty())
            {
                qWarning() << "[handle] There is no serial key, username and password.";
                return;
            }
            emit sigConnect(serialKey, userName, password,
                            mandatoryConnParams[ADDRESS_KEY].toString(),
                            uint16_t(mandatoryConnParams[PORT_KEY].toInt()), updateRouteTable);
        }
        else
        {
            emit sigConnectNoAuth( mandatoryConnParams[ADDRESS_KEY].toString(),
                                  uint16_t(mandatoryConnParams[PORT_KEY].toInt()));
        }
    }
}
