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

void SignInScreen::setupInputState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Sign in");
}

void SignInScreen::setupConnectingState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Service connecting...");
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
    QPushButton *btnSignIn   = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME ); Q_ASSERT(btnSignIn);
    QLineEdit   *edtEmail    = a_widget->findChild<QLineEdit  *>(EDT_EMAIL_NAME   ); Q_ASSERT(edtEmail);
    QLineEdit   *edtPassword = a_widget->findChild<QLineEdit  *>(EDT_PASSWORD_NAME); Q_ASSERT(edtPassword);
    QLabel *lblEmailError    = a_widget->findChild<QLabel     *>("lblEmailError")  ; Q_ASSERT(lblEmailError);
    QLabel *lblPasswordError = a_widget->findChild<QLabel     *>("lblPasswordError");Q_ASSERT(lblPasswordError);

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

