#include "StartScreen.h"

#include "StyledDropShadowEffect.h"
#include "AppController.h"

//const QString StartScreen::SCREEN_NAME = "StartScreen";

StartScreen::StartScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::StartScreen>();
}

void StartScreen::setupStateMachine()
{
    Q_ASSERT(stateMachine().check());

    assignWidgetPropertyForState<QWidget>(stateMachine().ctlConnecting, BTN_SIGN_IN_NAME, "text", "Service connecting..." );

    stateMachine().ctlConnecting->assignProperty(this, "enabled", false);
    stateMachine().ctlConnecting->assignProperty(this, "enabled", true);

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
    m_btnSignIn = a_widget->findChild<QPushButton*>(BTN_SIGN_IN_NAME); Q_ASSERT(m_btnSignIn);
    m_btnSignUp = a_widget->findChild<QPushButton*>(BTN_SIGN_UP_NAME); Q_ASSERT(m_btnSignUp);

    m_btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(m_btnSignIn));

    connect(m_btnSignIn, &QPushButton::clicked, [this]{
        emit this->transitionTo_SignIn();
        Q_ASSERT(false);

    });
}
