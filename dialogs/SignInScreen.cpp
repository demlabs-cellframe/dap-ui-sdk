#include "SignInScreen.h"

#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"

const QString SignInScreen::SCREEN_NAME = "StartScreen";

SignInScreen::SignInScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SignInScreen>();
}

QString SignInScreen::screenName()
{
    return SCREEN_NAME;
}

void SignInScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton *btnSignIn = a_widget->findChild<QPushButton*>("btnSignIn");
    QLineEdit *edtEmail = a_widget->findChild<QLineEdit*>("edtEmail");
    QLineEdit *edtPassword = a_widget->findChild<QLineEdit*>("edtPassword");
    QLabel *lblEmailError = a_widget->findChild<QLabel*>("lblEmailError");
    QLabel *lblPasswordError = a_widget->findChild<QLabel*>("lblPasswordError");


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
}

