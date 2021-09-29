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
  connect (&_signal(), &KelGuiStyleManager::cssStyleChanged,
           this, &KelGuiSwitchStyleManager::styleUpdated);
  connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
           this, &KelGuiSwitchStyleManager::styleUpdated);
}

KelGuiSwitchStyleManager::KelGuiSwitchStyleManager (KelGuiSwitchStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  setCssStyle("sw-ui");
  connect (&_signal(), &KelGuiStyleManager::cssStyleChanged,
           this, &KelGuiSwitchStyleManager::styleUpdated);
  connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
           this, &KelGuiSwitchStyleManager::styleUpdated);
}

KelGuiSwitchStyleManager::~KelGuiSwitchStyleManager()
{

}

/*-----------------------------------------*/
