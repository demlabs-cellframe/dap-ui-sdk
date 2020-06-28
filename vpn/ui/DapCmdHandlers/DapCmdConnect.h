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
    void sendCmdConnect(const QString& a_addr, quint16 a_port,
                        const QString& a_user= QString(), const QString& a_pswd = QString(),  const QString& a_serial= QString());


    void sendCmdDisconnect();
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void errorMessage(const QString& message);
};

