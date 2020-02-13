#include "SignInScreen.h"

#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"

const QString SignInScreen::SCREEN_NAME = "SignIn";

SignInScreen::SignInScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::SignInScreen>();

    initScreen(this);
}


QString SignInScreen::screenName()
{
    return SCREEN_NAME;
}

void SignInScreen::setEmail(const QString &a_email)
{
    setChildProperties(EDT_EMAIL_NAME, "text", a_email);
}

void SignInScreen::setPassword(const QString &a_password)
{
    setChildProperties(EDT_PASSWORD_NAME, "text", a_password);
}

void SignInScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton *btnSignIn   = a_widget->findChild<QPushButton*>(BTN_SIGN_IN      );
    QLineEdit   *edtEmail    = a_widget->findChild<QLineEdit*>  (EDT_EMAIL_NAME   );
    QLineEdit   *edtPassword = a_widget->findChild<QLineEdit*>  (EDT_PASSWORD_NAME);
    QLabel *lblEmailError    = a_widget->findChild<QLabel*>("lblEmailError");
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

    connect(edtEmail   , SIGNAL(textEdited(const QString&)), this, SIGNAL(emailEdited   (const QString&)));
    connect(edtPassword, SIGNAL(textEdited(const QString&)), this, SIGNAL(passwordEdited(const QString&)));
    connect(btnSignIn  , SIGNAL(clicked()), this, SIGNAL(signInRequest()));

}

