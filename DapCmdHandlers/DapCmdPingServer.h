#ifndef DAPCMDPINGSERVERHANDLER_H
#define DAPCMDPINGSERVERHANDLER_H

#include <QObject>
#include "DapCmdClientAbstract.h"

class DapCmdPingServer : public DapCmdClientAbstract
{
    Q_OBJECT
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    explicit DapCmdPingServer(QObject *parent = nullptr);
    void sendPingCmd(const QString& host, quint16 port);
signals:

public slots:
};

#endif // DAPCMDPINGSERVERHANDLER_H
