#include "NavigationPanelBack.h"

#include "ui_NavigationPanelBack.h"
#include "Utilz.h"



const QString NavigationPanelBack::SCREEN_NAME = "NavigationPanelBack";

NavigationPanelBack::NavigationPanelBack(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create<Ui::NavigationPanelBack>();

    AdaptiveScreen::initScreen(this);
}

QString NavigationPanelBack::screenName()
{
    return NavigationPanelBack::SCREEN_NAME;
}

void NavigationPanelBack::initVariantUi(QWidget *a_widget)
{
    QPushButton* btnBack; Utils::findChild<QPushButton*>(a_widget, BTN_BACK, btnBack);

    connect(btnBack, &QPushButton::clicked, this, &NavigationPanelBack::goBack);
}
