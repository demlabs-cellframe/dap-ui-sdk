#include "NavigationPanelBack.h"

#include "ui_NavigationPanelBack.h"
#include "Utilz.h"
#include "defines.h"


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
    this->connectBtnToSignall(BTN_BACK, &NavigationPanelBack::goBack, a_widget);
}

#ifndef ANDROID
void NavigationPanelBack::setBackState(bool a_backState)
{
    if (a_backState == m_backState)
        return;
    m_backState = a_backState;

    this->setChildProperties(BTN_BACK, Properties::VISIBLE, a_backState);
}

bool NavigationPanelBack::backState()
{
    return m_backState;
}
#endif
