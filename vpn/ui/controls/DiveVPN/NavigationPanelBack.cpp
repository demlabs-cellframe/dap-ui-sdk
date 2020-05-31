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
   QPushButton* btnBug           = a_widget->findChild<QPushButton*>("btnBug");              Q_ASSERT(btnBug);
   QPushButton* btnSettings       = a_widget->findChild<QPushButton*>("btnSettings");          Q_ASSERT(btnSettings);

   connect(btnSettings, &QPushButton::clicked,this,&NavigationPanelBack::goToSettings);
   connect(btnBug     , &QPushButton::clicked,this,&NavigationPanelBack::goToBugReport);
   // this->connectBtnToSignall(BTN_BACK, &NavigationPanelBack::goBack, a_widget);
}

#ifndef ANDROID
void NavigationPanelBack::setBackState(bool a_backState)
{
    if (a_backState == m_backState)
        return;
    m_backState = a_backState;

   // this->setChildProperties(BTN_BACK, Properties::VISIBLE, a_backState);
}

bool NavigationPanelBack::backState()
{
    return m_backState;
}
#endif
