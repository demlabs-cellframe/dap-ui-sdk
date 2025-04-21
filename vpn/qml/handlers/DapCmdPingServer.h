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
    void sendPingCmd (const QString& host, quint16 port, const QString &a_name = QString(), const QString& instanceId = QString());
signals:
    void sigPingReceived(QString, quint16, quint16, QString);

public slots:
};

#endif // DAPCMDPINGSERVERHANDLER_H
