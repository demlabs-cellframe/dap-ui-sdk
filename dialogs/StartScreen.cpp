#include "StartScreen.h"

#include "StyledDropShadowEffect.h"

StartScreen::StartScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::StartScreen>();
}

void StartScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation)

    QPushButton *btnSignIn = a_w->findChild<QPushButton*>("btnSignIn");
    CustomLineHeightLabel *lblMessage = a_w->findChild<CustomLineHeightLabel*>("lblMessage");

    Q_ASSERT(btnSignIn);
    Q_ASSERT(lblMessage);

    btnSignIn->setGraphicsEffect(new StyledDropShadowEffect(btnSignIn));

    connect(btnSignIn, &QPushButton::clicked, [this]{
        emit this->loginScreenTransition();
    });
}
