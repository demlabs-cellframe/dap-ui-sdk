#ifndef SETTINGSMOREMENU_H
#define SETTINGSMOREMENU_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QMenu>

#include "DapServerInfo.h"
#include "DialogEditServer.h"

namespace Ui {
class SettingsMoreMenu;
}

class SettingsMoreMenu : public QMenu
{
    Q_OBJECT
public:
    explicit SettingsMoreMenu(QWidget *parent = nullptr);
    ~SettingsMoreMenu();

    const DapServerInfo *curentServer() const;              //const pointer to server wich this menu will change
    void setCurentServer(const DapServerInfo *a_server);

    QToolButton *parentButton() const;                      //pointer to button wich executed menu
    void setParentButton(QToolButton *parentButton);

protected:
    virtual void hideEvent(QHideEvent *);


public slots:
    void restore();
    void edit();
    void remove();

signals:
    void editClicked(const DapServerInfo *a_server);

private:
    Ui::SettingsMoreMenu *ui;

    const DapServerInfo *m_curentServer;
    QToolButton *m_parentButton;

    DialogEditServer *m_dlgEditServer {Q_NULLPTR};
};

#endif // SETTINGSMOREMENU_H
