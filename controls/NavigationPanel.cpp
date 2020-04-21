#include "NavigationPanel.h"
#include "Utils.h"


#ifdef Q_OS_ANDROID
#include "DefaultMultiScreen.h"
#include "NavigationPanelBack.h"
#include "NavigationPanelMain.h"

NavigationPanel::NavigationPanel(QWidget *parent)
    : DefaultMultiScreen(parent)
{
}

#else
#include "ui_NavigationPanel.h"

NavigationPanel::NavigationPanel(QWidget *parent)
    : AdaptiveWidget(parent)
{
    this->create<Ui::NavigationPanel>();
    setState(States::Main);
}

#endif

void NavigationPanel::initVariantUi(QWidget *a_widget)
{
#ifdef ANDROID
    DefaultMultiScreen::initVariantUi(a_widget);
#else
    this->connectBtnToSignall(BTN_BUG     , &NavigationPanel::goToBugReport, a_widget);
    this->connectBtnToSignall(BTN_HELP    , &NavigationPanel::goToFAQ      , a_widget);
    this->connectBtnToSignall(BTN_SETTINGS, &NavigationPanel::goToSettings , a_widget);
#endif
}

void NavigationPanel::setState(NavigationPanel::States a_state)
{
#ifdef ANDROID

    if (a_state == States::Main)
        this->activateScreen<NavigationPanelMain>();

    else if (a_state == States::Back)
        this->activateScreen<NavigationPanelBack>();

#else
#endif
}
