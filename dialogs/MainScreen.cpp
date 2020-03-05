#include "MainScreen.h"
#include "StyledDropShadowEffect.h"
#include <QTimer>
#include "ComboBoxPopup.h"
#include "CustomButtonDelegateFactory.h"

#include "defines.h"
#include "DapDataLocal.h"

const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());
    create<Ui::MainScreen>();
}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
    this->setChildProperties(LBL_STATUS_MESSAGE_NAME, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE_NAME, Properties::STATE, a_state);
    this->updateChildStyle  (LBL_STATUS_MESSAGE_NAME);

    this->setChildProperties(BTN_SWITCH_NAME, Properties::STATE, a_state);
    this->updateChildStyle  (BTN_SWITCH_NAME);

    this->setChildProperties(CBB_SERVER_NAME, Properties::ENABLED, a_state == ConnectionStates::Disconnected);
    this->setChildProperties(BTN_SWITCH_NAME, Properties::ENABLED, (a_state == ConnectionStates::Disconnected || a_state == ConnectionStates::Connected));
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    ComboBox       *cbbServer        = a_widget->findChild<ComboBox   *>(CBB_SERVER_NAME)        ; Q_ASSERT(cbbServer       );
    QLabel         *lblStatusMessage = a_widget->findChild<QLabel     *>(LBL_STATUS_MESSAGE_NAME); Q_ASSERT(lblStatusMessage);
    QPushButton    *btnSwitch        = a_widget->findChild<QPushButton*>(BTN_SWITCH_NAME)        ; Q_ASSERT(btnSwitch       );

    connect(btnSwitch, &QPushButton::clicked, [this]{
        emit connectionSwitched();
    });


    if (m_serversModel)
        cbbServer->setModel(m_serversModel);
    else
    {
        m_serversModel = cbbServer->model();

        for (DapServerInfo& server :DapDataLocal::me()->servers())
            cbbServer->addItem(server.name);

        connect(DapDataLocal::me(), &DapDataLocal::serverAdded, [cbbServer](const DapServerInfo& a_serverInfo){
            cbbServer->addItem(a_serverInfo.name);
        });

        connect(DapDataLocal::me(), SIGNAL(serversCleared()), cbbServer, SLOT(clear()));
    }

    cbbServer->QComboBox::setCurrentText(DapDataLocal::me()->serverName());

    ComboBoxPopup* cbPopup = new ComboBoxPopup(a_widget);
    cbPopup->setDelegateFactory<CustomButtonDelegateFactory>();
    cbPopup->setCaption("Choose server");
    cbbServer->setPopup(cbPopup);
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
