#include "DapCmdAbstract.h"

QMap<DapJsonCmdType, QString> DapCmdAbstract::cmdString = {
    {DapJsonCmdType::CONNECTION, "connection"},
    {DapJsonCmdType::AUTHORIZE, "authorize"},
    {DapJsonCmdType::STATS, "stats"},
    {DapJsonCmdType::GET_STATES, "get states"},
    {DapJsonCmdType::GET_USER_DATA, "get user data"}, // Get user data (username, password, server address)
    // Get data about the last connection (connection start time)
    {DapJsonCmdType::GET_LAST_CONNECTION_DATA, "get last connection data"},
    // Getting last connection data (connection start time)
    {DapJsonCmdType::TAP_DRIVER, "tap driver"},
    {DapJsonCmdType::PING_SERVER, "ping server"},
    {DapJsonCmdType::SEND_SERVER_DATA, "send server data"},
    {DapJsonCmdType::SEND_BUG_REPORT, "send bug report"},
    {DapJsonCmdType::BUG_REPORTS_STATUS, "bug reports status request"},
    {DapJsonCmdType::SEND_SIGN_UP_REQUEST, "send sign up request"},
    {DapJsonCmdType::GET_NEWS, "get news"},
    {DapJsonCmdType::GET_SERVERS_LIST, "get servers list"},
    {DapJsonCmdType::CREATE_ANDROID_TUNNEL, "create android tunnel"},
    {DapJsonCmdType::SEND_RESET_SERIAL_KEY_REQUEST, "request reset license key"},
    {DapJsonCmdType::REQUEST_PURCHASE_VERIFY, "verify purchase"}, // it won't hurt in default build variant
    {DapJsonCmdType::CLIENT_INFO, "client info"},
    {DapJsonCmdType::CDB_CTL, "cdb list"},
    {DapJsonCmdType::NODE_INFO, "node info"},
    {DapJsonCmdType::UPDATE_OPERATION, "update operation"},
    {DapJsonCmdType::GEO_IP, "geo ip"},
    {DapJsonCmdType::DATA_LOCAL, "data local"},
    {DapJsonCmdType::CONNECTION_PROBLEM, "connection problem"},
};

void DapCmdAbstract::sendCmd(const QJsonObject * obj)
{
    QJsonObject cmdObj {{"command", commandToString(m_cmd) }};
    if(obj != Q_NULLPTR) {
        cmdObj.insert(m_side == Side::CLIENT ? "params" : "result", *obj);
    }
    //qDebug() << "[sendCmd] cmdObj: " << cmdObj;
    emit send(QJsonDocument(cmdObj).toJson());
}

DapJsonCmdType DapCmdAbstract::stringToCommand(const QString& cmd)
{
    return cmdString.key(cmd);
}

QString DapCmdAbstract::commandToString(DapJsonCmdType command)
{
    QString sCmd = cmdString.value(command);
    if (sCmd == "") {
        throw std::runtime_error("Unknown DapCommand. Add his to cmdStrings!");
    }
    return sCmd;
}

DapJsonCmdType DapCmdAbstract::getCommand(const QJsonDocument& doc)
{
    if(doc.object().find("command") == doc.object().end()){
        qCritical() << "Not found command field";
        return DapJsonCmdType::UNKNOWN;
    }
    return stringToCommand(doc.object()["command"].toString());
}
