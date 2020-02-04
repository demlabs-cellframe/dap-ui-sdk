#include "PasswordRecoveryScreen.h"

#include "ui_PasswordRecoveryScreen.h"

PasswordRecoveryScreen::PasswordRecoveryScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::PasswordRecoveryScreen>();
}

void PasswordRecoveryScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w)
    Q_UNUSED(a_rotation)

        QPushButton *btnSendMail = a_w->findChild<QPushButton*>("btnSendMail");
        QLabel *lblStatusMessage = a_w->findChild<QLabel*>("lblStatusMessage");
        QLabel *lblStatusMessage_2 = a_w->findChild<QLabel*>("lblStatusMessage_2");
        QLineEdit *edtEmail = a_w->findChild<QLineEdit*>("edtEmail");
        QLabel *lblEmailError = a_w->findChild<QLabel*>("lblEmailError");

        Q_ASSERT(btnSendMail);
        Q_ASSERT(lblStatusMessage);
        Q_ASSERT(lblStatusMessage_2);
        Q_ASSERT(edtEmail);
        Q_ASSERT(lblEmailError);

        static int presBtn = 0;

        edtEmail->setPlaceholderText("Your email");
        btnSendMail->setGraphicsEffect(new StyledDropShadowEffect(btnSendMail));
        lblEmailError->setVisible(false);
        lblStatusMessage->setText("<p style='line-height:130%;'>Please enter your email address.<br>We will send you an email to<br>reset your password.</p>");

        lblStatusMessage_2->setVisible(false);
        lblStatusMessage_2->setText("<p style='line-height:127%;'>Email sent. And other information<br>in this paragraph.</p>");
        connect(btnSendMail,&QPushButton::clicked,[=]{
            switch (presBtn%3)
            {
            case 0:
                presBtn++;
                lblEmailError->setVisible(true);
                break;
            case 1:
                lblStatusMessage->setVisible(false);
                lblStatusMessage_2->setVisible(true);
                edtEmail->setVisible(false);
                lblEmailError->setVisible(false);
                btnSendMail->setText("Back");
                presBtn++;
                break;
            case 2:
                lblStatusMessage->setVisible(true);
                lblStatusMessage_2->setVisible(false);
                edtEmail->setVisible(true);
                btnSendMail->setText("Send email");
                presBtn++;
                break;
            }

        });
}
