#ifndef DAPCMDCONNHANDLER_H
#define DAPCMDCONNHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdConnect : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
public:
    explicit DapCmdConnect(QObject *parent = nullptr);
    virtual ~DapCmdConnect() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendCmdError(const QString errorMsg); // TODO ENUM with errors and code in response
    void sendCmdError(int a_errorCode,const QString a_errorMsg);
signals:
    void sigConnect(const QString& serial, const QString& user, const QString& password, const QString& address, uint16_t port);


    void sigConnectNoAuth(const QString& address, uint16_t port);
    void sigDisconnect();
    void sigRestartService();
};

#endif // DAPCMDCONNHANDLER_H
