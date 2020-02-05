#include "StartScreen.h"

#include "StyledDropShadowEffect.h"
#include "AppController.h"

//const QString StartScreen::SCREEN_NAME = "StartScreen";

StartScreen::StartScreen(QWidget *a_parent)
    : Screen(a_parent)
{
    create<Ui::StartScreen>();
}

//void StartScreen::activateScreen()
//{
//    AppController::mainWindow()->
//}

//QString StartScreen::screenName()
//{
//    return SCREEN_NAME;
//}

void StartScreen::initUi(QWidget *a_w, ScreenInfo::Rotation a_rotation)
{
    btnSignIn = a_w->findChild<QPushButton*>("btnSignIn"); Q_ASSERT(btnSignIn);
    btnSignUp = a_w->findChild<QPushButton*>("btnSignUp"); Q_ASSERT(btnSignUp);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));

    connect(btnSignIn, &QPushButton::clicked, [this]{
        emit this->transitionTo_SignIn();
    });
}

void StartScreen::setupStateMachine()
{
//    QList<QWidget*> widgets {btnSignIn, btnSignUp};
//    QStringList widgetNames {"edtEmail", "edtPassword", "btnSignIn", "lblSignUp", "lblForgetPass"}
//    assignWidgetPropertyForState<QWidget>(AppController::stateMachine()->start, )

}
