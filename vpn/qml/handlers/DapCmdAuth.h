#pragma once
#include <QObject>
#include <QDebug>
#include "DapCmdClientAbstract.h"

class DapCmdAuth : public DapCmdClientAbstract
{
    Q_OBJECT
public:
    explicit DapCmdAuth(QObject *parent = nullptr);
    void sendCmdAuth(const QString& addr, quint16 port,
                        const QString& user, const QString& pswd);
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
signals:
    void errorMessage(const QString& message);
};

