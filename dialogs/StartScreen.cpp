#include "StartScreen.h"

#include "StyledDropShadowEffect.h"
#include "AppController.h"

const QString StartScreen::SCREEN_NAME = "Start";

StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::StartScreen>();

    initScreen(this);
}

QString StartScreen::screenName()
{
    return StartScreen::SCREEN_NAME;
}


void StartScreen::setupConnectingState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Service connecting...");
    setEnabled(false);
}

void StartScreen::setupConnectedState()
{
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Sign in");
    setEnabled(true);
}

void StartScreen::initVariantUi(QWidget *a_widget)
{
    QPushButton* l_btnSignIn = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME); Q_ASSERT(l_btnSignIn);
    QPushButton* l_btnSignUp = a_widget->findChild<QPushButton*>(BTN_SIGN_UP_NAME); Q_ASSERT(l_btnSignUp);

    l_btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(l_btnSignIn));

    connect(l_btnSignIn, &QPushButton::clicked, [this]
    {
        qDebug()<<"clicked";
        emit this->transitionTo_SignIn();
    });

    connect(l_btnSignUp, &QPushButton::clicked, [l_btnSignUp, this]
    {
        qDebug()<<"go to signUp clicked";
        emit this->transitionTo_SignUp();
    });

}
