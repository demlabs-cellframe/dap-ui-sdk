#ifndef DAPCMDSYSTEMTRAYHANDLER_H
#define DAPCMDSYSTEMTRAYHANDLER_H

#include <QObject>
#include <QJsonObject>
#include "DapCmdServiceAbstract.h"

class DapCmdSystemTray : public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    static const QString actionParam;
public:
    explicit DapCmdSystemTray(QObject *parent = nullptr);
    virtual ~DapCmdSystemTray() override;

    void handle(const QJsonObject* params) override;
public slots:
    void sendInterfaceRequest(QString interfaceName);
signals:
    void interfaceRequest(QString interfaceName);
};

#endif // DAPCMDSYSTEMTRAYHANDLER_H
