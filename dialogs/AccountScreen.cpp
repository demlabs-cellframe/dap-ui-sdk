#include "AccountScreen.h"

AccountScreen::AccountScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::AccountScreen>();
}

void AccountScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{
    //... Will be evaluted by create()
}
