#include "AccountScreen.h"

AccountScreen::AccountScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::AccountScreen>();

    QLabel *lblSwitchLeftText = a_sw->findChild<QLabel*>("lblSwitchLeftText");
    QLabel *lblSwitchRightText = a_sw->findChild<QLabel*>("lblSwitchRightText");
    QPushButton *btnSwitch = a_sw->findChild<QPushButton*>("btnSwitchButton");
    CustomPlacementButton *btnLogOut = a_sw->findChild<CustomPlacementButton*>("btnLogOut");
    QPushButton *btnChangePlan  = a_sw->findChild<QPushButton*>("btnChangePlan");

    Q_ASSERT(lblSwitchLeftText);
    Q_ASSERT(lblSwitchRightText);
    Q_ASSERT(btnSwitch);
    Q_ASSERT(btnLogOut);
    Q_ASSERT(btnChangePlan);

    btnLogOut->setObjectName("btnLogOut");
    btnLogOut->setText("Logout");
    btnLogOut->setImagePosition(ImagePos::Right);

    btnChangePlan->setGraphicsEffect(new StyledDropShadowEffect(btnChangePlan));

    ///The default Radiobutton value for checking the operation
    btnSwitch->setChecked(false);
    lblSwitchLeftText->setEnabled(false);

    connect(btnSwitch,&QPushButton::clicked,[=]{
        lblSwitchLeftText->setEnabled(btnSwitch->isChecked());
        lblSwitchRightText->setEnabled(!btnSwitch->isChecked());
    });
}

void AccountScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w)
    Q_UNUSED(a_rotation)


}
