/* INCLUDES */
#include "dapguicheckboxstylemanager.h"
#include "dapguicheckbox.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiCheckBoxStyleManager::DapGuiCheckBoxStyleManager (QWidget *parent)
  : DapGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiCheckBoxStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate);
    }
}

DapGuiCheckBoxStyleManager::DapGuiCheckBoxStyleManager (DapGuiCheckBoxStyleManager &&src)
  : DapGuiStyleManager ((DapGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiCheckBoxStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate);
    }
}

DapGuiCheckBoxStyleManager::~DapGuiCheckBoxStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void DapGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate()
{
  /* get push button ptr */
  DapGuiCheckBox *cb  = dynamic_cast<DapGuiCheckBox *> (m_widget);
  if (!cb)
    return;

  /* compile style */
  QString s =
    QString (
      "*\n{"
      "%5}\n"

      "QCheckBox::indicator\n{"
      "border-image: url(\"%1\");%4}\n"

      "QCheckBox::indicator:hover\n{"
      "border-image: url(\"%2\");}\n"

      "QCheckBox::indicator:checked\n{"
      "border-image: url(\"%3\");}\n")
    .arg (
      "://gfx/check_btn_off.png",
      "://gfx/check_btn_on.png",
      "://gfx/check_btn_on.png",
      styleByClassName ("ccb_indicator"),
      styleByClassName ("ccb_textstyle font16 darkblue normal lato"));

  cb->setStyleSheet (s);
}

/*-----------------------------------------*/
