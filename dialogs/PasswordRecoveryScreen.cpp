#include "PasswordRecoveryScreen.h"

#include "ui_PasswordRecoveryScreen.h"
#include <QStyle>

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
        CustomLineHeightLabel *lblStatusMessage = a_w->findChild<CustomLineHeightLabel*>("lblStatusMessage");
        QLineEdit *edtEmail = a_w->findChild<QLineEdit*>("edtEmail");
        QLabel *lblEmailError = a_w->findChild<QLabel*>("lblEmailError");

        Q_ASSERT(btnSendMail);
        Q_ASSERT(lblStatusMessage);
        Q_ASSERT(edtEmail);
        Q_ASSERT(lblEmailError);

        static int presBtn = 0;

        edtEmail->setPlaceholderText("Your email");
        btnSendMail->setGraphicsEffect(new StyledDropShadowEffect(btnSendMail));
        lblEmailError->setVisible(false);
        lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to<br>reset your password.");

        connect(btnSendMail,&QPushButton::clicked,[=]{
            switch (presBtn%3)
            {
            case 0:
                presBtn++;
                lblEmailError->setVisible(true);

                edtEmail->setProperty("state",1);
                edtEmail->style()->unpolish(edtEmail);
                edtEmail->style()->polish(edtEmail);
                break;
            case 1:
                lblStatusMessage->setProperty("state",1);
                lblStatusMessage->style()->unpolish(lblStatusMessage);
                lblStatusMessage->style()->polish(lblStatusMessage);
                lblStatusMessage->setText("Email sent. And other information<br>in this paragraph.");

                edtEmail->setVisible(false);
                lblEmailError->setVisible(false);
                btnSendMail->setText("Back");
                presBtn++;
                break;
            case 2:
                lblStatusMessage->setProperty("state",0);
                lblStatusMessage->style()->unpolish(lblStatusMessage);
                lblStatusMessage->style()->polish(lblStatusMessage);
                lblStatusMessage->setText("Please enter your email address.<br>We will send you an email to<br>reset your password.");

                edtEmail->setProperty("state",0);
                edtEmail->style()->unpolish(edtEmail);
                edtEmail->style()->polish(edtEmail);

                edtEmail->setVisible(true);
                btnSendMail->setText("Send email");
                presBtn++;
                break;
            }

        });
}
