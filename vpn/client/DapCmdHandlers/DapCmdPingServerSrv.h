#ifndef DapCmdPingServerSrvHANDLER_H
#define DapCmdPingServerSrvHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"

class DapCmdPingServerSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
public:

    explicit DapCmdPingServerSrv(QObject *parent = nullptr);

    /// Virtual destructor.
    virtual ~DapCmdPingServerSrv() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

signals:
    void sendPingServerTime(const QByteArray& ba);
public slots:
};

#endif // DapCmdPingServerSrvHANDLER_H
