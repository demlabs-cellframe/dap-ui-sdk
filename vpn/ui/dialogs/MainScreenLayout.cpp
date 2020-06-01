#include "MainScreenLayout.h"

#include "StyledDropShadowEffect.h"

const QString MainScreenLayout::SCREEN_NAME = "MainLayout";

MainScreenLayout::MainScreenLayout(QWidget *a_parent)
    :MultiScreenAbstract(a_parent)
{
    create<Ui::MainScreenLayout>();

    AdaptiveScreen::initScreen(this);
}

QString MainScreenLayout::screenName()
{
    return MainScreenLayout::SCREEN_NAME;
}

NavigationPanel *MainScreenLayout::findNavigationPanel(QWidget* a_widget /*= nullptr*/)
{
    if (!a_widget)
        a_widget = this->variant();

    NavigationPanel *panel = a_widget->findChild<NavigationPanel*>();
    Q_ASSERT(panel);

    return panel;
}

/**
* @brief Form initialization.
* @param a_w Window GUI widget.
* @param a_rotation Device display orientation.
*/
void MainScreenLayout::initVariantUi(QWidget *a_widget)
{
    this->initChangingWidget(a_widget);
}

