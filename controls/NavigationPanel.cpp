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
}

#endif

void NavigationPanel::initVariantUi(QWidget *a_widget)
{
#ifdef Q_OS_ANDROID
    DefaultMultiScreen::initVariantUi(a_widget);
#else

    QPushButton *btnBug = Utils::findChild<QPushButton*>(a_widget, BTN_BUG );
    QPushButton *btnHelp= Utils::findChild<QPushButton*>(a_widget, BTN_HELP);

    this->connect(btnBug , &QPushButton::clicked, this, &NavigationPanel::goToBugReport);
    this->connect(btnHelp, &QPushButton::clicked, this, &NavigationPanel::goToFAQ      );

#endif
    setState(States::Main);
}

void NavigationPanel::setState(NavigationPanel::States a_state)
{
#ifdef Q_OS_ANDROID

    if (a_state == States::Main)
        this->activateScreen<NavigationPanelMain>();

    else if (a_state == States::Back)
        this->activateScreen<NavigationPanelBack>();

#else
#endif
}
