#include "DapCmdSystemTray.h"

void DapCmdSystemTray::handle(const QJsonObject *params)
{
    Q_UNUSED(params);

    if(params->contains("client_show_interface"))
    {
        qDebug() << "Request for show interface" << params->value("client_show_interface").toString();
        if (params->value("client_show_interface").toString() == "setting")
            emit showSettingInterface();
        if (params->value("client_show_interface").toString() == "statistic")
            emit showStatisticInterface();
        if (params->value("client_show_interface").toString() == "login")
            emit showLoginInterface();
        if (params->value("client_show_interface").toString() == "dashboard")
            emit showDashboardInterface();
        if (params->value("client_show_interface").toString() == "client_started")
            emit clientFound();
        if (params->value("client_show_interface").toString() == "client_exited")
            emit clientClosed();
        if (params->value("client_show_interface").toString() == "apllication_quit")
            emit quitRequest();
        if (params->value("client_show_interface").toString() == "tray_application_running")
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
    response["client_show_interface"] = interfaceName;
    sendCmd(&response);
}

void DapCmdSystemTray::clientStarted()
{
    QJsonObject response;
    response["client_show_interface"] = "client_started";
    sendCmd(&response);
}

void DapCmdSystemTray::clientExited()
{
    QJsonObject response;
    response["client_show_interface"] = "client_exited";
    sendCmd(&response);
}

void DapCmdSystemTray::trayApplicationStarted()
{
    sendShowInterface("tray_application_running");
}
