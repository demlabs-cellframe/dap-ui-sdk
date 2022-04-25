#include "DapCmdSystemTray.h"

void DapCmdSystemTray::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains("clientShowInterface"))
    {
        qDebug() << "Request for show interface" << params->value("clientShowInterface").toString();
        if (params->value("clientShowInterface").toString() == "setting")
            emit showSettingInterface();
        if (params->value("clientShowInterface").toString() == "statistic")
            emit showStatisticInterface();
        if (params->value("clientShowInterface").toString() == "login")
            emit showLoginInterface();
        if (params->value("clientShowInterface").toString() == "dashboard")
            emit showDashboardInterface();
        if (params->value("clientShowInterface").toString() == "clientStarted")
            emit clientFound();
        if (params->value("clientShowInterface").toString() == "clientExited")
            emit clientClosed();
        if (params->value("clientShowInterface").toString() == "apllication_quit")
            emit quitRequest();
        if (params->value("clientShowInterface").toString() == "tray_application_running")
            emit trayApplicationFound();
    }
}

void DapCmdSystemTray::sendShowSettingInterface()
{
    sendShowInterface("setting");
}

void DapCmdSystemTray::sendShowStatisticInterface()
{
    sendShowInterface("statistic");
}

void DapCmdSystemTray::sendShowLoginInterface()
{
    sendShowInterface("login");
}

void DapCmdSystemTray::sendShowDashboardInterface()
{
    sendShowInterface("dashboard");
}

void DapCmdSystemTray::apllicationQuitRequest()
{
    sendShowInterface("apllication_quit");
}

/// @param address Server address.
void DapCmdSystemTray::sendShowInterface(const QString &interfaceName)
{
    qDebug() << "cmd sendShowInterface" << interfaceName;
    QJsonObject response;
    response["clientShowInterface"] = interfaceName;
    sendCmd(&response);
}

void DapCmdSystemTray::clientStarted()
{
    QJsonObject response;
    response["clientShowInterface"] = "clientStarted";
    sendCmd(&response);
}

void DapCmdSystemTray::clientExited()
{
    QJsonObject response;
    response["clientShowInterface"] = "clientExited";
    sendCmd(&response);
}

void DapCmdSystemTray::trayApplicationStarted()
{
    sendShowInterface("tray_application_running");
}
