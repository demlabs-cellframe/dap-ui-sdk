#include "DapCmdAbstract.h"

QMap<DapJsonCmdType, QString> DapCmdAbstract::cmdString = {
    {DapJsonCmdType::CONNECTION, "connection"},
    {DapJsonCmdType::STATS, "stats"},
    {DapJsonCmdType::GET_STATES, "get states"},
    {DapJsonCmdType::GET_USER_DATA, "get user data"}, // Get user data (username, password, server address)
    // Get data about the last connection (connection start time)
    {DapJsonCmdType::GET_LAST_CONNECTION_DATA, "get last connection data"},
    // Getting last connection data (connection start time)
    {DapJsonCmdType::TAP_DRIVER, "tap driver"},
    {DapJsonCmdType::PING_SERVER, "ping server"},
    {DapJsonCmdType::GET_SERVERS_LIST, "get servers list"}
};

void DapCmdAbstract::sendCmd(const QJsonObject * obj)
{
    QJsonObject cmdObj {{"command", commandToString(m_cmd) }};
    if(obj != Q_NULLPTR) {
        cmdObj.insert(m_side == Side::CLIENT ? "params" : "result", *obj);
    }
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
