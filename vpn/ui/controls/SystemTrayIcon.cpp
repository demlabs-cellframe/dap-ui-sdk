#include "SystemTrayIcon.h"

SystemTrayIcon::SystemTrayIcon(QWidget* a_parent)
    : QSystemTrayIcon(a_parent)
    , m_trayMenu            (new QMenu(a_parent))
    , m_actionConnect       (new QAction(tr("Connect"),this))
    , m_actionConnectionInfo(new QAction(tr("Connection info"), this))
    , m_actionAccount       (new QAction(tr("Obtain serial key"),this))
    , m_actionSettings      (new QAction(tr("Settings"), this))
    , m_actionQuit          (new QAction(tr("Quit"), this))
{
    connect(this, &QSystemTrayIcon::activated, this, &SystemTrayIcon::iconClicked);
    connect(m_actionConnect,        &QAction::triggered, this, &SystemTrayIcon::connectClicked);
    connect(m_actionConnectionInfo, &QAction::triggered, this, &SystemTrayIcon::connectionInfoClicked);
    connect(m_actionAccount,        &QAction::triggered, this, &SystemTrayIcon::accountClicked);
    connect(m_actionSettings,       &QAction::triggered, this, &SystemTrayIcon::settingsClicked);
    connect(m_actionQuit,           &QAction::triggered, this, [this]()
    {
        m_actionQuit->setEnabled(false);
        emit quitClicked();
    });

    m_trayMenu->addActions(QList<QAction*>{m_actionConnect,
                                           m_actionConnectionInfo,
                                           m_actionAccount,
                                           m_actionSettings,
                                           m_actionQuit});

    this->setIcon(QIcon(QString(":/pics/%1.png").arg(DAP_BRAND)));
    this->setContextMenu(m_trayMenu);
    this->show();
}

void SystemTrayIcon::setConnectionState(ConnectionState a_state)
{
    if (m_connectionState != a_state)
    {
        m_connectionState = a_state;
        switch (m_connectionState)
        {
        case Disconnected:
            m_actionConnect->setText(tr("Connect"));
            m_actionAccount->setText(tr("Obtain serial key"));
            m_actionConnect->setEnabled(true);
            m_actionConnectionInfo->setEnabled(false);
            m_actionAccount->setEnabled(true);
            m_actionSettings->setEnabled(true);
            break;
        case Connected:
            m_actionConnect->setText(tr("Disconnect"));
            m_actionAccount->setText(tr("Account"));
            m_actionConnect->setEnabled(true);
            m_actionConnectionInfo->setEnabled(true);
            m_actionAccount->setEnabled(true);
            m_actionSettings->setEnabled(true);
            break;
        case ConnectingToService:
        case ServersListLoading:
            m_actionConnect->setText(tr("Connecting to service..."));
            m_actionAccount->setText(tr("Obtain serial key"));
            m_actionConnect->setEnabled(false);
            m_actionConnectionInfo->setEnabled(false);
            m_actionAccount->setEnabled(false);
            m_actionSettings->setEnabled(false);
            break;
        case Disconnecting:
            m_actionConnect->setText(tr("Disconnecting..."));
            m_actionAccount->setText(tr("Obtain serial key"));
            m_actionConnect->setEnabled(false);
            m_actionConnectionInfo->setEnabled(false);
            m_actionAccount->setEnabled(true);
            m_actionSettings->setEnabled(true);
            break;
        case ServerChanging:
            m_actionConnect->setText(tr("Changing server..."));
            m_actionAccount->setText(tr("Account"));
            m_actionConnect->setEnabled(false);
            m_actionConnectionInfo->setEnabled(false);
            m_actionAccount->setEnabled(true);
            m_actionSettings->setEnabled(true);
            break;
        case Reconnecting:
        case Connecting:
            m_actionConnect->setText(tr("Connecting..."));
            m_actionAccount->setText(tr("Obtain serial key"));
            m_actionConnect->setEnabled(false);
            m_actionConnectionInfo->setEnabled(false);
            m_actionAccount->setEnabled(true);
            m_actionSettings->setEnabled(true);
            break;
        }
    }
    else return;
}
