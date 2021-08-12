#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H
#include <QSystemTrayIcon>
#include <QMenu>
#include "vpnDefine.h"


class SystemTrayIcon:public QSystemTrayIcon
{
    Q_OBJECT
public:
    SystemTrayIcon(QWidget *a_parent = nullptr);
signals:
    void iconClicked(QSystemTrayIcon::ActivationReason);
    void connectClicked();
    void connectionInfoClicked();
    void accountClicked();
    void settingsClicked();
    void quitClicked();
public slots:
    void setConnectionState(ConnectionState a_state);
private:
    QMenu*   m_trayMenu;
    QAction* m_actionConnect;
    QAction* m_actionConnectionInfo;
    QAction* m_actionAccount;
    QAction* m_actionSettings;
    QAction* m_actionQuit;
    ConnectionState m_connectionState = ConnectionState::Disconnected;
};

#endif // SYSTEMTRAYICON_H
