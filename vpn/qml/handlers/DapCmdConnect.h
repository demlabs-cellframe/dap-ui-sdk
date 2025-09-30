#pragma once

#include <QObject>
#include <QDebug>
#include "DapCmdClientAbstract.h"

class DapCmdConnect : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdConnect(QObject *parent = nullptr);

public slots:
    void sendCmdConnect(const QString& a_addr, quint16 a_port, const QString& a_user = QString(), const QString& a_pswd = QString(),
                        const QString& a_serial = QString(), const bool a_updateRouteTable = true,
                        const QString& a_runScriptPath = QString(), const bool a_runAfterConnect = false);
    void sendCmdConnectByOrder(const QString& a_addr);


    void sendCmdDisconnect();
    void sendCmdRestartService(bool if_running = false);
  void sendCmdSetNoCdbMode(bool enabled);
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void errorMessage(const QString& message);
    void errorMessageWithCode(const QString& message);
    void connectionFail();
};

