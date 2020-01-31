#include "ForgetPasswordScreen.h"

#include "ui_ForgetPasswordScreen.h"

ForgetPasswordScreen::ForgetPasswordScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::ForgetPasswordScreen>();
}

void ForgetPasswordScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    Q_UNUSED(a_w)
    Q_UNUSED(a_rotation)
}
