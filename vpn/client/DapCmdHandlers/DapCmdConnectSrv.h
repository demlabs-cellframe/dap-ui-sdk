#ifndef DAPCMDCONNHANDLER_H
#define DAPCMDCONNHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdConnectSrv : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
public:
    explicit DapCmdConnectSrv(QObject *parent = nullptr);
    virtual ~DapCmdConnectSrv() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendCmdError(const QString errorMsg); // TODO ENUM with errors and code in response
    void sendCmdError(int a_errorCode,const QString a_errorMsg);
signals:
    void sigConnect(const QString& serial, const QString& user, const QString& password, const QString& address, uint16_t port);


    void sigConnectNoAuth(const QString& address, uint16_t port);
    void sigDisconnect();
    void sigRestartService(bool if_runnning);
};

#endif // DAPCMDCONNHANDLER_H
