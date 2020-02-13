#include "StartScreen.h"

#include "StyledDropShadowEffect.h"
#include "AppController.h"

const QString StartScreen::SCREEN_NAME = "Start";

StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::StartScreen>();

    initScreen(this);

    this->setupStateMachine();
}

QString StartScreen::screenName()
{
    return SCREEN_NAME;
}

StartScreen::StateLinks* StartScreen::statesLinks()
{
    return &this->m_statesLinks;
}

void StartScreen::setupStateMachine()
{
    Q_ASSERT(statesLinks()->check());


    assignWidgetPropertyForState<QPushButton>(statesLinks()->ctlConnecting, BTN_SIGN_IN_NAME, "text", "Service connecting..." );

    statesLinks()->ctlConnecting->assignProperty(this, "enabled", false);
    statesLinks()->ctlConnected->assignProperty(this, "enabled", true);
}

//void StartScreen::activateScreen()
//{
//    AppController::mainWindow()->
//}

//QString StartScreen::screenName()
//{
//    return SCREEN_NAME;
//}

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

    connect(l_btnSignUp, &QPushButton::clicked, [this]
    {
        qDebug()<<"go to signUp clicked";
        emit this->transitionTo_SignUp();
    });

}
