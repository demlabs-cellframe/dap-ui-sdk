#include "NavigationPanelMain.h"

#include "ui_NavigationPanelMain.h"
#include "Utils.h"

const QString NavigationPanelMain::SCREEN_NAME = "NavigationPanelMain";

NavigationPanelMain::NavigationPanelMain(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create<Ui::NavigationPanelMain>();

    AdaptiveScreen::initScreen(this);
}

QString NavigationPanelMain::screenName()
{
    return NavigationPanelMain::SCREEN_NAME;
}

void NavigationPanelMain::initVariantUi(QWidget *a_widget)
{
    QPushButton* btSettings; Utils::findChild(a_widget, BTN_SETTINGS, btSettings);
    QPushButton* btBug     ; Utils::findChild(a_widget, BTN_BUG     , btBug);
    QPushButton* btHelp    ; Utils::findChild(a_widget, BTN_HELP    , btHelp);

    connect(btSettings, &QPushButton::clicked, this, &NavigationPanelMain::goToSettings);
    connect(btBug     , &QPushButton::clicked, this, &NavigationPanelMain::goToBugReport);
    connect(btHelp    , &QPushButton::clicked, this, &NavigationPanelMain::goToFAQ);

    btHelp->setVisible(false);
}
