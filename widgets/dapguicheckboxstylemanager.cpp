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

      "QCheckBox::indicator\n{%1%4}\n"

      "QCheckBox::indicator:hover\n{%2}\n"

      "QCheckBox::indicator:checked\n{%3}\n")
    .arg (
      styleByClassName ("ccb_off-light"),
      styleByClassName ("ccb_on-light"),
      styleByClassName ("ccb_on-light"),
      styleByClassName ("ccb_indicator"),
      styleByClassName ("ccb_textstyle font16 darkblue normal lato"));

  cb->setStyleSheet (s);
}

/*-----------------------------------------*/
