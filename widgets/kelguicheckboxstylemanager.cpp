/* INCLUDES */
#include "kelguicheckboxstylemanager.h"
#include "kelguicheckbox.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiCheckBoxStyleManager::KelGuiCheckBoxStyleManager (QWidget *parent)
  : KelGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiCheckBoxStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate);
    }
}

KelGuiCheckBoxStyleManager::KelGuiCheckBoxStyleManager (KelGuiCheckBoxStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiCheckBoxStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate);
    }
}

KelGuiCheckBoxStyleManager::~KelGuiCheckBoxStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void KelGuiCheckBoxStyleManager::forcedCheckboxStyleUpdate()
{
  /* get push button ptr */
  KelGuiCheckBox *cb  = dynamic_cast<KelGuiCheckBox *> (m_widget);
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
