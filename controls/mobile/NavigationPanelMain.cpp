#include "NavigationPanelMain.h"

#include "ui_NavigationPanelMain.h"

const QString NavigationPanelMain::SCREEN_NAME = "NavigationPanelMain";

NavigationPanelMain::NavigationPanelMain(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create<Ui::NavigationPanelMain>();
}

QString NavigationPanelMain::screenName()
{
    return NavigationPanelMain::SCREEN_NAME;
}

void NavigationPanelMain::initVariantUi(QWidget *a_widget)
{
    // TODO
}
