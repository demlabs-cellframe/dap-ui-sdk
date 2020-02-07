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

    QComboBox *cbCountry = a_w->findChild<QComboBox*>("cbCountry");
    QLabel *lblStatusMessage = a_w->findChild<QLabel*>("lblStatusMessage");
    QPushButton *btnSwitch = a_w->findChild<QPushButton*>("btnSwitch");

    Q_ASSERT(cbCountry);
    Q_ASSERT(lblStatusMessage);
    Q_ASSERT(btnSwitch);


    cbCountry->setGraphicsEffect(new StyledDropShadowEffect(cbCountry));
    static QTimer timeButton;
    lblStatusMessage->setText("Not connected");

    connect(btnSwitch,&QPushButton::clicked,[=]{
        lblStatusMessage->setProperty("stateLbl",1);
        lblStatusMessage->style()->unpolish(lblStatusMessage);
        lblStatusMessage->style()->polish(lblStatusMessage);

        lblStatusMessage->setText("Connecting...");
        btnSwitch->setProperty("stateBtn",1);
        btnSwitch->style()->unpolish(btnSwitch);
        btnSwitch->style()->polish(btnSwitch);

        timeButton.start(1500);

    });
    connect(&timeButton,&QTimer::timeout,[=]{
        btnSwitch->setProperty("stateBtn",2);
        btnSwitch->style()->unpolish(btnSwitch);
        btnSwitch->style()->polish(btnSwitch);

        lblStatusMessage->setProperty("stateLbl",2);
        lblStatusMessage->style()->unpolish(lblStatusMessage);
        lblStatusMessage->style()->polish(lblStatusMessage);
        lblStatusMessage->setText("Connected");
    });
}
