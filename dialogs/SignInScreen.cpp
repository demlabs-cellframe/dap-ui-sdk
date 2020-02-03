#include "SignInScreen.h"

#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"

SignInScreen::SignInScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::SignInScreen>();
}

void SignInScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    QPushButton *btnSignIn = a_w->findChild<QPushButton*>("btnSignIn");
    QLineEdit *edtEmail = a_w->findChild<QLineEdit*>("edtEmail");
    QLineEdit *edtPassword = a_w->findChild<QLineEdit*>("edtPassword");
    QLabel *lblEmailError = a_w->findChild<QLabel*>("lblEmailError");
    QLabel *lblPasswordError = a_w->findChild<QLabel*>("lblPasswordError");


    Q_ASSERT(btnSignIn);
    Q_ASSERT(edtEmail);
    Q_ASSERT(edtPassword);
    Q_ASSERT(lblEmailError);
    Q_ASSERT(lblPasswordError);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));

    edtEmail->setPlaceholderText("Email");
    edtPassword->setPlaceholderText("Password");
    edtPassword->setEchoMode(QLineEdit::Password);
    lblEmailError->setVisible(false);
    lblPasswordError->setVisible(false);

    connect(btnSignIn,&QPushButton::clicked,[=]{
        lblEmailError->setVisible(true);
        lblPasswordError->setVisible(true);
    });

    Q_UNUSED(a_rotation)
}
