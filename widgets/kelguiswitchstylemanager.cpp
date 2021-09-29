/* INCLUDES */
#include "kelguiswitchstylemanager.h"
#include "kelguiswitch.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiSwitchStyleManager::KelGuiSwitchStyleManager (QWidget *parent)
  : KelGuiStyleManager (parent)
{
  setCssStyle("sw-ui");
}

KelGuiSwitchStyleManager::KelGuiSwitchStyleManager (KelGuiSwitchStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  setCssStyle("sw-ui");
}

KelGuiSwitchStyleManager::~KelGuiSwitchStyleManager()
{

}

/*-----------------------------------------*/
