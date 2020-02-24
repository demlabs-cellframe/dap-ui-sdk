#include "MainScreen.h"
#include "StyledDropShadowEffect.h"
#include <QTimer>
#include "CustomComboBoxPopup.h"
#include "ListModel.h"
#include "defines.h"
#include "DapDataLocal.h"

const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::MainScreen>();
}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::STATE, a_state);
    this->updateChildStyle  (LBL_STATUS_MESSAGE);

    this->setChildProperties(BTN_SWITCH, Properties::STATE, a_state);
    this->updateChildStyle  (BTN_SWITCH);

    this->setChildProperties(CBB_SERVER, Properties::ENABLED, a_state == ConnectionStates::Disconnected);
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    CustomComboBox *cbbServer        = a_widget->findChild<CustomComboBox*>(CBB_SERVER)        ; Q_ASSERT(cbbServer       );
    QLabel         *lblStatusMessage = a_widget->findChild<QLabel        *>(LBL_STATUS_MESSAGE); Q_ASSERT(lblStatusMessage);
    QPushButton    *btnSwitch        = a_widget->findChild<QPushButton   *>(BTN_SWITCH)        ; Q_ASSERT(btnSwitch       );

    connect(btnSwitch, &QPushButton::clicked, [this]{
        emit connectionSwitched();
    });

    for (DapServerInfo& server :DapDataLocal::me()->servers())
    {
        cbbServer->addItem(server.name);
    }

    cbbServer->QComboBox::setCurrentText(DapDataLocal::me()->serverName());

//    QIcon icon(":/pics/flag.svg");
//    cbbServer->setItemIcon(0,icon);
}

QString MainScreen::statusText(ConnectionStates a_state)
{
    switch (a_state)
    {
        case ConnectionStates::Disconnected:
            return  "Not connected";
        case ConnectionStates::Connecting:
            return "Connecting...";
        case ConnectionStates::Connected:
            return  "Connected";
        case ConnectionStates::Disconnecting:
            return  "Server down";
    }
    return QString();
}
