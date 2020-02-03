#include "StartScreen.h"

#include "StyledDropShadowEffect.h"

StartScreen::StartScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::StartScreen>();
}

void StartScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    QPushButton *btnSignIn = a_w->findChild<QPushButton*>("btnSignIn");
    Q_ASSERT(btnSignIn);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));

    connect(btnSignIn, &QPushButton::clicked, [this]{
        emit this->transitionTo_Login();
    });
}
