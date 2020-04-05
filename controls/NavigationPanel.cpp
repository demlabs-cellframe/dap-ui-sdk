#include "NavigationPanel.h"



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
    CustomPlacementButton *btnLogOut = a_widget->findChild<CustomPlacementButton*>(BTN_LOGOUT_NAME); Q_ASSERT(btnLogOut);

    btnLogOut->setImagePosition(ImagePos::Right);

#endif
    setState(States::Main);
}

void NavigationPanel::setState(NavigationPanel::States a_state)
{
#ifdef Q_OS_ANDROID
    switch (a_state)
    {
        case States::Main:
            this->activateScreen<NavigationPanelMain>();
            break;
        case States::Back:
            this->activateScreen<NavigationPanelBack>();
            break;
    }
#else
#endif
}
