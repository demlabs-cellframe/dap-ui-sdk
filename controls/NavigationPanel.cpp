#include "NavigationPanel.h"

#include "ui_NavigationPanel.h"

NavigationPanel::NavigationPanel(QWidget *parent)
    : QWidget(parent)
{
    Ui::NavigationPanel* panelUI = new Ui::NavigationPanel();
    panelUI->setupUi(this);
}
