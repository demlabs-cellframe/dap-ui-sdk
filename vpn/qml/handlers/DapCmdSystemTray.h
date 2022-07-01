/****************************************************************************
**
** This file is part of the DapVpn project.
**
** The class implements the command handler for receiving user data from
** the service.
**
****************************************************************************/

#ifndef DAPCMDSYSTEMTRAYHANDLER_H
#define DAPCMDSYSTEMTRAYHANDLER_H

#include <QObject>
#include <QJsonValue>

#include "DapCmdServiceAbstract.h"

class DapCmdSystemTray: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    DapCmdSystemTray(QObject *parent = nullptr) :
        DapCmdServiceAbstract(DapJsonCmdType::CLIENT_INFO, parent) {
    }
    /// Virtual destructor.
    virtual ~DapCmdSystemTray() override { }
    /// Process command.
    /// @param params Command parameters.
    void handle(const QJsonObject* params) override;

    /// @param address Server address.
    void sendShowSettingInterface();
    void sendShowStatisticInterface();
    void sendShowLoginInterface();
    void sendShowDashboardInterface();
    void clientStarted();
    void trayApplicationStarted();
    void clientExited();
    void apllicationQuitRequest();

signals:
    void showSettingInterface();
    void showStatisticInterface();
    void showLoginInterface();
    void showDashboardInterface();
    void trayApplicationFound();
    void clientFound();
    void clientClosed();
    void quitRequest();

private:
    void sendShowInterface(const QString &interfaceName);
};

#endif // DAPCMDSYSTEMTRAYHANDLER_H
