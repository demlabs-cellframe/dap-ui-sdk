#include "MainScreenLayout.h"

#include "StyledDropShadowEffect.h"

MainScreenLayout::MainScreenLayout(QObject * a_parent, QStackedWidget * a_sw)
    :DapUIAnimationScreenAbstract(a_parent, a_sw)
{
    create<Ui::MainScreenLayout>();
}

/**
* @brief Form initialization.
* @param a_w Window GUI widget.
* @param a_rotation Device display orientation.
*/
void MainScreenLayout::initUi(QWidget *a_w, ScreenRotation a_rotation)
{
   Q_UNUSED(a_rotation);

   initChangedScreen(a_w);

   //create and activate ScreenLogin
//   m_screenLogin = activateScreen<ScreenLogin>();
}
