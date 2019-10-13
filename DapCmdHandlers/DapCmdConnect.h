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
    void sendCmdErrorAuth(const QString errorMsg); // TODO ENUM with errors and code in response
signals:
    void sigConnect(const QString& user, const QString& password, const QString& address, uint16_t port);
    void sigDisconnect();
};

#endif // DAPCMDCONNHANDLER_H
