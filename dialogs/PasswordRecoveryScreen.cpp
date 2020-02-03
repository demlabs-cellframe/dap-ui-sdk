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
        QLineEdit *edtEmail = a_w->findChild<QLineEdit*>("edtEmail");
        QLabel *lblEmailError = a_w->findChild<QLabel*>("lblEmailError");

        Q_ASSERT(btnSendMail);
        Q_ASSERT(lblStatusMessage);
        Q_ASSERT(edtEmail);
        Q_ASSERT(lblEmailError);

        edtEmail->setPlaceholderText("Your email");
        btnSendMail->setGraphicsEffect(new StyledDropShadowEffect(btnSendMail));

        lblStatusMessage->setText("<p style='line-height:145%;'>Please enter your email address.<br>We will send you an email to<br>reset your password.</p>");

        connect(btnSendMail,&QPushButton::clicked,[=]{
            lblEmailError->setMaximumHeight(1000);
            lblStatusMessage->setText("<p style='line-height:145%;'>Email sent. And other information<br>in this paragraph.</p>");
        });
}
