#ifndef DAPJSONPARAMAS_H
#define DAPJSONPARAMAS_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QDebug>

enum class DapJsonCmdType
{
    CONNECTION,
    STATS,
    GET_STATES,
    GET_USER_DATA,
    GET_LAST_CONNECTION_DATA,
    LAST_CONNECTION_DATA,
    CREATE_ANDROID_TUNNEL,
    GET_SERVERS_LIST,
    TAP_DRIVER,
    PING_SERVER,
    UNKNOWN
};

class DapJsonParams
{
public:
    enum Params {
        VALUE, STATE_NAME, READ_BYTES, WRITE_BYTES,
        MESSAGE, ADDRESS, PORT, USER, PASSWORD, DISCONNECT,
        READ_PACKAGE, WRITE_PACKAGE, LAST_CONNECTION, GATEWAY, UPSTREAM_SOCKET,
        ACTION, HOST
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
