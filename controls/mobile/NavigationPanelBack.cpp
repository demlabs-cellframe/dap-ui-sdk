#include "NavigationPanelBack.h"

#include "ui_NavigationPanelBack.h"
#include "Utils.h"


const QString NavigationPanelBack::SCREEN_NAME = "NavigationPanelMain";

NavigationPanelBack::NavigationPanelBack(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create<Ui::NavigationPanelBack>();
}

QString NavigationPanelBack::screenName()
{
    return NavigationPanelBack::SCREEN_NAME;
}

void NavigationPanelBack::initVariantUi(QWidget *a_widget)
{
    QPushButton* btnBack = nullptr;
    Utils::findObjectChild(a_widget, BTN_BACK, btnBack);



}
