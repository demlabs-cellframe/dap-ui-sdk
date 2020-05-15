#include "NavigationPanel.h"

#include "defines.h"
#include "NavigationPanelBack.h"
#include "NavigationPanelMain.h"

NavigationPanel::NavigationPanel(QWidget *a_parent)
    :PanelParentClass(a_parent)
{

}

void NavigationPanel::setBackState(bool a_backState)
{
    if (a_backState == m_backState)
        return;
    m_backState = a_backState;

#ifdef ANDROID
    if (a_backState)
        this->activateScreen<NavigationPanelBack>();
    else
        this->activateScreen<NavigationPanelMain>();

#else
    this->setChildProperties(BTN_BACK, Properties::VISIBLE, a_backState);
#endif

}

bool NavigationPanel::backState()
{
    return m_backState;
}
