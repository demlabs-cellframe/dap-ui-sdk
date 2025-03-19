#include "DapCmdConnect.h"
#include <QMap>
#include <QDebug>
#include "DapServiceDataLocal.h"
#include "DapSession.h"
#include "DapSerialKeyData.h"

DapCmdConnect::DapCmdConnect(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::CONNECTION, parent) 
{}

DapCmdConnect::~DapCmdConnect()
{}

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
    DapCmdServiceAbstract::handle(params);

    if (params->contains(ACTION_KEY)) {
        QString req = params->value(ACTION_KEY).toString("");
        if (req == "Disconnect") {
            qDebug() << "DapCmdConnect::Disconnect signal";
            emit sigDisconnect();
            return;
        } else if (req == "RestartService") {
            qDebug() << "DapCmdConnect::RestartService signal";
            emit sigRestartService(false);
            return;
        } else if (req == "RestartServiceIfRunning") {
            qDebug() << "DapCmdConnect::RestartServiceIfRunning signal";
            emit sigRestartService(true);
            return;
        }
    }

    QMap<QString, QJsonValue> mandatoryConnParams = {
        {ADDRESS_KEY, params->value(ADDRESS_KEY)},
        {PORT_KEY, params->value(PORT_KEY)},
        {UPDATE_ROUTE_TABLE, params->value(UPDATE_ROUTE_TABLE)},
        {RUN_SCRIPT_PATH, params->value(RUN_SCRIPT_PATH)},
        {RUN_SCRIPT_AFTER_CONNECT, params->value(RUN_SCRIPT_AFTER_CONNECT)}
    };

    if (!mandatoryConnParams[ADDRESS_KEY].isString() || !mandatoryConnParams[PORT_KEY].isDouble()) {
        qWarning() << "Missing or invalid mandatory connection parameters";
        return;
    }

    bool updateRouteTable = mandatoryConnParams[UPDATE_ROUTE_TABLE].toBool(true);
    QString serialKey = "";

    if (params->contains("serial")) {
        serialKey = params->value("serial").toString();
    }

    if (serialKey.isEmpty()) {
        serialKey = DapServiceDataLocal::instance()->serialKeyData()->serialKey();
    }

    uint16_t port = uint16_t(mandatoryConnParams[PORT_KEY].toInt());
    QString address = mandatoryConnParams[ADDRESS_KEY].toString();

    QString runScriptPath = mandatoryConnParams[RUN_SCRIPT_PATH].toString();

    if (!runScriptPath.isEmpty()) {
        bool runAfterConnect = mandatoryConnParams[RUN_SCRIPT_AFTER_CONNECT].toBool(true);
        qDebug() << "Running script path:" << runScriptPath << "Run after connect:" << runAfterConnect;
        emit sigSetRunScript(runScriptPath, runAfterConnect);
    }

    qDebug() << "Address:" << address << ", Port:" << port << ", UpdateRouteTable:" << updateRouteTable;
    qDebug() << "SerialKey:" << (serialKey.isEmpty() ? "No serial key provided" : "Serial key provided");

    if (!serialKey.isEmpty()) {
        emit sigConnect(serialKey, "", "", address, port, updateRouteTable);
    } else {
        sendCmdError("No serial key provided");
    }
}


