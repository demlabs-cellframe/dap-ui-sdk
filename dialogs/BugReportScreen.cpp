#include "BugReportScreen.h"



const QString BugReportScreen::SCREEN_NAME = "BugReport";

BugReportScreen::BugReportScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::BugReportScreen>();
}

QString BugReportScreen::screenName()
{
    return BugReportScreen::SCREEN_NAME;
}

void BugReportScreen::setState(ConnectionStates a_state)
{
    Q_UNUSED(a_state)
}

void BugReportScreen::initVariantUi(QWidget *a_widget)
{
    CustomLineHeightLabel *lblStatusMessage = a_widget->findChild<CustomLineHeightLabel*>("lblStatusMessage");
    QPushButton *btnSend = a_widget->findChild<QPushButton*>("btnSend");
    QLineEdit *edtEmail = a_widget->findChild<QLineEdit*>("edtEmail");
    CustomLineHeightTextEdit *edtMessage = a_widget->findChild<CustomLineHeightTextEdit*>("edtMessage");

    Q_ASSERT(lblStatusMessage);
    Q_ASSERT(btnSend);
    Q_ASSERT(edtEmail);
    Q_ASSERT(edtMessage);

    btnSend->setGraphicsEffect(new StyledDropShadowEffect(btnSend));

    lblStatusMessage->setVisible(false);

    edtEmail->setPlaceholderText("Your email");

    edtMessage->setPlaceholderText("Please describe the details of problem you faced. What actions did you take and what happened");

    connect(btnSend,&QPushButton::clicked,[=]{
        edtEmail->setVisible(false);
        edtMessage->setVisible(false);
        lblStatusMessage->setVisible(true);
        btnSend->setText("Back");
    });
}
