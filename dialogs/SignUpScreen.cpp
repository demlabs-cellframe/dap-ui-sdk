#include "SignUpScreen.h"

#include "ui_SignUpScreen.h"


SignUpScreen::SignUpScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::SignUpScreen>();
}

void SignUpScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w)
    Q_UNUSED(a_rotation)
}
