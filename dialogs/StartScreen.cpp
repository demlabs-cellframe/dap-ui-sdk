#include "StartScreen.h"

StartScreen::StartScreen(QObject * a_parent, QStackedWidget * a_sw)
    : DapUiScreen(a_parent, a_sw)
{
    create<Ui::StartScreen>();
}

void StartScreen::initUi(QWidget *a_w, DapUiScreen::ScreenRotation a_rotation)
{

}
