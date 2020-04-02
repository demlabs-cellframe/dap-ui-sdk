#include "MainScreenLayout.h"

#include "StyledDropShadowEffect.h"

const QString MainScreenLayout::SCREEN_NAME = "MainLayout";

MainScreenLayout::MainScreenLayout(QWidget *a_parent)
    :MultiScreenAbstract(a_parent)
{
    create<Ui::MainScreenLayout>();
}

QString MainScreenLayout::screenName()
{
    return MainScreenLayout::SCREEN_NAME;
}

/**
* @brief Form initialization.
* @param a_w Window GUI widget.
* @param a_rotation Device display orientation.
*/
void MainScreenLayout::initVariantUi(QWidget *a_widget)
{
   this->initChangingWidget(a_widget);

   //create and activate ScreenLogin
//   m_screenLogin = activateScreen<ScreenLogin>();
}
