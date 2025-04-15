#ifndef DAPJSONPARAMAS_H
#define DAPJSONPARAMAS_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QDebug>

enum class DapJsonCmdType
{
    AUTHORIZE,
    CONNECTION,
    STATS,
    GET_STATES,
    GET_USER_DATA,
    GET_LAST_CONNECTION_DATA,
    LAST_CONNECTION_DATA,
    CREATE_ANDROID_TUNNEL,
    GET_SERVERS_LIST,
    GET_NEWS,
    TAP_DRIVER,
    PING_SERVER,
    SEND_SERVER_DATA,
    SEND_BUG_REPORT,
    SEND_RESET_SERIAL_KEY_REQUEST,
    BUG_REPORTS_STATUS,
    SEND_SIGN_UP_REQUEST,
    REQUEST_PURCHASE_VERIFY,
    CLIENT_INFO,
    CDB_CTL,
    NODE_INFO,
    UPDATE_OPERATION,
    GEO_IP,
    DATA_LOCAL,
    UNKNOWN
};

class DapJsonParams
{
public:
    enum Params {
        VALUE, STATE_NAME,
        MESSAGE, ADDRESS, PORT, USER, PASSWORD, DISCONNECT,
        LAST_CONNECTION, GATEWAY, UPSTREAM_SOCKET,
        ACTION, HOST, CDB, INSTANCE_ID
    };
    static const QString& toString(Params p);
    static Params fromString(QString &param);

    static bool isParamsAvailable(const DapJsonCmdType& cmd,
                                  const Params& p);

    DapJsonParams() = delete;
    DapJsonParams(const DapJsonParams &) = delete;
private:
    /* For exampe: state can have parameter STATE_NAME
        Stats can have READ_BYTES, WRITE_BYTES etc. */
    static QMap<DapJsonCmdType, QVector<Params>> availableParams;
};

#endif // DAPJSONPARAMAS_H
