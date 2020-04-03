#include "StartScreen.h"
#include "AppController.h"

const QString StartScreen::SCREEN_NAME = "Start";

/// Overloaded constructor.
/// @param a_parent Parent.
StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{

    create<Ui::StartScreen>();
}

QString StartScreen::screenName()
{
    return StartScreen::SCREEN_NAME;
}


void StartScreen::setupConnectingState()
{
#ifdef Q_OS_ANDROID

#else
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Service connecting...");
    setEnabled(false);
#endif
}

void StartScreen::setupConnectedState()
{
#ifdef Q_OS_ANDROID

#else
    setChildProperties(BTN_SIGN_IN_NAME, "text", "Sign in");
    setEnabled(true);
#endif
}

void StartScreen::initVariantUi(QWidget *a_widget)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(a_widget)
#else
    QPushButton* l_btnSignIn = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME); Q_ASSERT(l_btnSignIn);
    QPushButton* l_btnSignUp = a_widget->findChild<QPushButton*>(BTN_SIGN_UP_NAME); Q_ASSERT(l_btnSignUp);

    l_btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(l_btnSignIn));

//l_btnSignIn->installEventFilter(new CustomEventFilter);

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
#endif
}
