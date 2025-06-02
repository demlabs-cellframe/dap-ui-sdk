#include "DapJsonParams.h"
#include "DapCmdAbstract.h"

using Params = DapJsonParams::Params;

QMap<DapJsonCmdType, QVector<Params>> DapJsonParams::availableParams {
    {DapJsonCmdType::CONNECTION, {Params::ADDRESS, Params::PORT,
                    Params::USER, Params::PASSWORD, Params::DISCONNECT},
    },
    {DapJsonCmdType::AUTHORIZE, {Params::ADDRESS, Params::PORT,
                    Params::USER, Params::PASSWORD, Params::DISCONNECT},
    },

    {DapJsonCmdType::GET_STATES, {}}, // GET_STATES Without params
    // Get user data (username, password, server address)
    {DapJsonCmdType::GET_USER_DATA, {}},
    // Send sserver data (server address, port)
    {DapJsonCmdType::SEND_SERVER_DATA, {}},
    // Get data about the last connection 
    {DapJsonCmdType::GET_LAST_CONNECTION_DATA, {}},
    // Getting last connection data (connection start time)
    {DapJsonCmdType::LAST_CONNECTION_DATA, {Params::LAST_CONNECTION}},
    // Create android tunnel on GUI part.
    {DapJsonCmdType::CREATE_ANDROID_TUNNEL, {Params::ADDRESS, Params::GATEWAY, Params::UPSTREAM_SOCKET}},
    // Get data about the last connection
    {DapJsonCmdType::TAP_DRIVER, {Params::VALUE}},
    {DapJsonCmdType::PING_SERVER, {Params::HOST, Params::PORT}},
    // Get/Set cdb servers list
    {DapJsonCmdType::CDB_CTL, {Params::CDB, Params::VALUE}},
};

bool DapJsonParams::isParamsAvailable(const DapJsonCmdType& cmd, const Params& p)
{
    if(!availableParams.contains(cmd)) {
        qWarning() << "Unknown DapJsonParams. Maybe add his to paramsName?";
        return false;
    }

    auto paramsList = availableParams.value(cmd);
    if(paramsList.isEmpty()) {
        qWarning() << "Something wrong params list for command:"
                   << DapCmdAbstract::commandToString(cmd) << "is empty!";
        return false;
    }

    return paramsList.contains(p);
}

const QString& DapJsonParams::toString(Params p) {
    static QMap<Params, QString> paramsName = {
        {VALUE, "value"},
        {STATE_NAME, "state_name"},
        {ADDRESS, "address"},
        {PORT, "port"},
        {HOST, "host"},
        {USER, "user"},
        {PASSWORD, "password"},
        {DISCONNECT, "disconnect"},
        {MESSAGE, "message"},
        {LAST_CONNECTION, "last_connection"},
        {GATEWAY, "gateway"},
        {UPSTREAM_SOCKET, "upstream_socket"},
        {CDB, "cdb"},
        {INSTANCE_ID, "instance_id"}
    };

    if (!paramsName.contains(p)) {
        throw std::runtime_error("Unknown DapJsonParams. Add his to paramsName!");
    }
    return paramsName[p];
}
