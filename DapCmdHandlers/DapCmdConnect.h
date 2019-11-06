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
    void sendCmdConnect(const QString& addr, quint16 port,
                        const QString& user= QString(), const QString& pswd = QString() );
    void sendCmdDisconnect();
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void errorMessage(const QString& message);
};

