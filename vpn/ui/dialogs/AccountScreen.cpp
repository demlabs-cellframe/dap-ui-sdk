#include "AccountScreen.h"

const QString AccountScreen::SCREEN_NAME = "Account";

AccountScreen::AccountScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::AccountScreen>();

}

QString AccountScreen::screenName()
{
    return AccountScreen::SCREEN_NAME;
}

void AccountScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)


}

void AccountScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)

}
//AccountScreen::AccountScreen(QObject * a_parent, QStackedWidget * a_sw)
//    : DapUiScreen(a_parent, a_sw)
//{
//    create<Ui::AccountScreen>();

//    QLabel *lblSwitchLeftText = a_sw->findChild<QLabel*>("lblSwitchLeftText");
//    QLabel *lblSwitchRightText = a_sw->findChild<QLabel*>("lblSwitchRightText");
//    QPushButton *btnSwitch = a_sw->findChild<QPushButton*>("btnSwitchButton");
//    QPushButton *btnChangePlan  = a_sw->findChild<QPushButton*>("btnChangePlan");

//    Q_ASSERT(lblSwitchLeftText);
//    Q_ASSERT(lblSwitchRightText);
//    Q_ASSERT(btnSwitch);
//    Q_ASSERT(btnChangePlan);

//    btnChangePlan->setGraphicsEffect(new StyledDropShadowEffect(btnChangePlan));

//    ///The default Radiobutton value for checking the operation
//    btnSwitch->setChecked(false);
//    lblSwitchLeftText->setEnabled(false);

//    connect(btnSwitch,&QPushButton::clicked,[=]{
//        lblSwitchLeftText->setEnabled(btnSwitch->isChecked());
//        lblSwitchRightText->setEnabled(!btnSwitch->isChecked());
//    });
//}

