#include "MainScreen.h"

#include "StyledDropShadowEffect.h"
#include <QTimer>

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

void MainScreen::initVariantUi(QWidget *a_widget)
{
    CustomComboBox *cbbCountry = a_widget->findChild<CustomComboBox*>("cbbCountry");
    QLabel *lblStatusMessage = a_widget->findChild<QLabel*>("lblStatusMessage");
    QPushButton *btnSwitch = a_widget->findChild<QPushButton*>("btnSwitch");

    Q_ASSERT(cbbCountry);
    Q_ASSERT(lblStatusMessage);
    Q_ASSERT(btnSwitch);


    cbbCountry->setGraphicsEffect(new StyledDropShadowEffect(cbbCountry));
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
