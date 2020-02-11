#include "MainScreen.h"

#include "StyledDropShadowEffect.h"
#include <QTimer>

MainScreen::MainScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::MainScreen>();
}

void MainScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation)

    CustomComboBox *cbbCountry = a_w->findChild<CustomComboBox*>("cbbCountry");
    QLabel *lblStatusMessage = a_w->findChild<QLabel*>("lblStatusMessage");
    QPushButton *btnSwitch = a_w->findChild<QPushButton*>("btnSwitch");

    Q_ASSERT(cbbCountry);
    Q_ASSERT(lblStatusMessage);
    Q_ASSERT(btnSwitch);

    cbbCountry->addItem("Natherlans");
    QIcon icon(":/pics/flag.svg");
    cbbCountry->setItemIcon(0,icon);
    static QTimer timeButton;
    lblStatusMessage->setText("Not connected");

    connect(btnSwitch,&QPushButton::clicked,[=]{
        lblStatusMessage->setProperty("state",1);
        lblStatusMessage->style()->unpolish(lblStatusMessage);
        lblStatusMessage->style()->polish(lblStatusMessage);

        lblStatusMessage->setText("Connecting...");
        btnSwitch->setProperty("state",1);
        btnSwitch->style()->unpolish(btnSwitch);
        btnSwitch->style()->polish(btnSwitch);
        btnSwitch->update();
        timeButton.start(1500);

    });
    connect(&timeButton,&QTimer::timeout,[=]{
        btnSwitch->setProperty("state",2);
        btnSwitch->style()->unpolish(btnSwitch);
        btnSwitch->style()->polish(btnSwitch);
        btnSwitch->update();
        lblStatusMessage->setProperty("state",2);
        lblStatusMessage->style()->unpolish(lblStatusMessage);
        lblStatusMessage->style()->polish(lblStatusMessage);
        lblStatusMessage->setText("Connected");
    });
}
