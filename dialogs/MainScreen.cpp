#include "MainScreen.h"
#include "StyledDropShadowEffect.h"
#include <QTimer>
#include "CustomComboBoxPopup.h"

#include "defines.h"

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
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    CustomComboBox *cbbCountry = a_widget->findChild<CustomComboBox*>("cbbCountry");
    QLabel *lblStatusMessage = a_widget->findChild<QLabel*>(LBL_STATUS_MESSAGE);
    QPushButton *btnSwitch = a_widget->findChild<QPushButton*>(BTN_SWITCH);

    Q_ASSERT(cbbCountry);
    Q_ASSERT(lblStatusMessage);
    Q_ASSERT(btnSwitch);

    cbbCountry->addItem("Natherlans");
    QIcon icon(":/pics/flag.svg");
    cbbCountry->setItemIcon(0,icon);

    connect(cbbCountry,&CustomComboBox::showCustomWindow,[=]{
        QList<DataModel> *dataList = new QList<DataModel>;
        DataModel tmpModel;
        for(int i = 0; i < 5;i++)
        {
            tmpModel.text = "kelvin-testnet.Cellframe";
            tmpModel.iconPath =":/pics/flag.svg";
            dataList->append(tmpModel);
        }

        CustomComboBoxPopup *s_comboBoxPopup = new CustomComboBoxPopup(a_widget);
        s_comboBoxPopup->setModel(dataList);
        s_comboBoxPopup->setTextCapture("Choose server");
        s_comboBoxPopup->show();
    });
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
    }

}
