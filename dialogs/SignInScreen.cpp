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
    Q_ASSERT(btnSignIn);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));
}
