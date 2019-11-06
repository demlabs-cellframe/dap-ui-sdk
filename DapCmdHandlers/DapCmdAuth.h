#pragma once
#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdAuth : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
public:
    explicit DapCmdAuth(QObject *parent = nullptr);
    virtual ~DapCmdAuth() override;

    void handle(const QJsonObject* params) override;
    void sendCmdErrorAuth(const QString errorMsg); // TODO ENUM with errors and code in response
signals:
    void sigAuth(const QString& user, const QString& password, const QString& address, uint16_t port);
};

