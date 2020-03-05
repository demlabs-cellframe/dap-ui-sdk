#include "NavigationPanel.h"

#include "ui_NavigationPanel.h"

NavigationPanel::NavigationPanel(QWidget *parent)
    : AdaptiveWidget(parent)
{
    this->create<Ui::NavigationPanel>();
}

void NavigationPanel::initVariantUi(QWidget *a_widget)
{
    CustomPlacementButton *btnLogOut = a_widget->findChild<CustomPlacementButton*>(BTN_LOGOUT_NAME); Q_ASSERT(btnLogOut);

    btnLogOut->setImagePosition(ImagePos::Right);
}
