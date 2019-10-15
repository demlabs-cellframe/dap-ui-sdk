#ifndef DAPCMDPINGSERVERHANDLER_H
#define DAPCMDPINGSERVERHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"

class DapCmdPingServer: public DapCmdServiceAbstract
{
    Q_OBJECT
public:

    explicit DapCmdPingServer(QObject *parent = nullptr);

    /// Virtual destructor.
    virtual ~DapCmdPingServer() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

signals:
    void sendPingServerTime(const QByteArray& ba);
public slots:
};

#endif // DAPCMDPINGSERVERHANDLER_H
