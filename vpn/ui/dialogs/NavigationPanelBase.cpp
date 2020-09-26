#include "NavigationPanelBase.h"

#include "defines.h"
#include "NavigationPanelBack.h"
#include "NavigationPanelMain.h"

NavigationPanelBase::NavigationPanelBase(QWidget *a_parent)
    :PanelParentClass(a_parent)
{

}

void NavigationPanelBase::setBackState(bool a_backState)
{
    if (a_backState == m_backState)
        return;
    m_backState = a_backState;

#ifndef USE_THO_LAYOUTS
    if (a_backState)
        this->activateScreen<NavigationPanelBack>();
    else
        this->activateScreen<NavigationPanelMain>();

#else
    this->setChildProperties(BTN_BACK, Properties::VISIBLE, a_backState);
#endif

}

bool NavigationPanelBase::backState()
{
    return m_backState;
}

