#ifndef DAPCMDCONNHANDLER_H
#define DAPCMDCONNHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdConnect : public DapCmdServiceAbstract
{
    Q_OBJECT

public:
    explicit DapCmdConnect(QObject *parent = nullptr);
    virtual ~DapCmdConnect() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendCmdError(const QString errorMsg); // TODO ENUM with errors and code in response
    void sendCmdError(int a_errorCode,const QString a_errorMsg);
signals:
    void sigConnect(const QString& serial, const QString& user, const QString& password, const QString& address, uint16_t port, bool updateRouteTable);


    void sigConnectNoAuth(const QString& address, uint16_t port);
    void sigDisconnect();
    void sigRestartService(bool if_runnning);
    void sigSetRunScript(const QString& runScriptPath, bool runAfterConnect);
    void sigSetNoCdbMode(bool enabled);

private:
    const QString ACTION_KEY = "action";
    const QString SERIAL_KEY = "serial";
    const QString USER_KEY = "user";
    const QString PASSWORD_KEY = "password";
    const QString ADDRESS_KEY = "address";
    const QString PORT_KEY = "port";
    const QString UPDATE_ROUTE_TABLE = "updateRouteTable";
    const QString RUN_SCRIPT_PATH = "run_script_path";
    const QString RUN_SCRIPT_AFTER_CONNECT = "run_script_after_connect";

    const QString CODE_KEY = "code";
    const QString MESSAGE_KEY = "message";
    const QString ERROR_KEY = "error";
};

#endif // DAPCMDCONNHANDLER_H
