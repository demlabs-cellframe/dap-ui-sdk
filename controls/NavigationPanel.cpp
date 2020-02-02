#include "NavigationPanel.h"

#include "ui_NavigationPanel.h"

NavigationPanel::NavigationPanel(QWidget *parent)
    : QWidget(parent)
{
    Ui::NavigationPanel* panelUI = new Ui::NavigationPanel();
    panelUI->setupUi(this);

        CustomPlacementButton *btnLogOut = parent->findChild<CustomPlacementButton*>("btnLogOut");

        Q_ASSERT(btnLogOut);

        btnLogOut->setObjectName("btnLogOut");
        btnLogOut->setText("Logout");
        btnLogOut->setImagePosition(ImagePos::Right);
}
