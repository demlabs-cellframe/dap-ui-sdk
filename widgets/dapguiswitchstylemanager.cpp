/* INCLUDES */
#include "dapguiswitchstylemanager.h"
#include "dapguiswitch.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiSwitchStyleManager::DapGuiSwitchStyleManager (QWidget *parent)
  : DapGuiStyleManager (parent)
{
  setCssStyle("sw-ui");
  connect (&_signal(), &DapGuiStyleManager::cssStyleChanged,
           this, &DapGuiSwitchStyleManager::styleUpdated);
  connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
           this, &DapGuiSwitchStyleManager::styleUpdated);
}

DapGuiSwitchStyleManager::DapGuiSwitchStyleManager (DapGuiSwitchStyleManager &&src)
  : DapGuiStyleManager ((DapGuiStyleManager &&)src)
{
  setCssStyle("sw-ui");
  connect (&_signal(), &DapGuiStyleManager::cssStyleChanged,
           this, &DapGuiSwitchStyleManager::styleUpdated);
  connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
           this, &DapGuiSwitchStyleManager::styleUpdated);
}

DapGuiSwitchStyleManager::~DapGuiSwitchStyleManager()
{

}

/*-----------------------------------------*/
