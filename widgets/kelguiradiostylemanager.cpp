/* INCLUDES */
#include "kelguiradiostylemanager.h"
#include "kelguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiRadioStyleManager::KelGuiRadioStyleManager (QWidget *parent)
  : KelGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiRadioStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiRadioStyleManager::forcedRadioStyleUpdate);
    }
}

KelGuiRadioStyleManager::KelGuiRadioStyleManager (KelGuiRadioStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiRadioStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiRadioStyleManager::forcedRadioStyleUpdate);
    }
}

KelGuiRadioStyleManager::~KelGuiRadioStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void KelGuiRadioStyleManager::forcedRadioStyleUpdate()
{
  /* get push button ptr */
  KelGuiRadio *rad  = dynamic_cast<KelGuiRadio *> (m_widget);
  if (!rad)
    return;

  /* compile style */
  QString s =
    QString (
      "*\n{"
      "border: none;"
      "%5}\n"

      "QRadioButton{border: none;}\n"
      "KelGuiRadio{border: none;}\n"

      "QRadioButton::indicator\n{"
      "border: none;"
      "border-image: url(\"%1\");%4}\n"

      "QRadioButton::indicator:hover\n{"
      "border: none;"
      "border-image: url(\"%2\");}\n"

      "QRadioButton::indicator:checked\n{"
      "border: none;"
      "border-image: url(\"%3\");}\n")
    .arg (
      "://gfx/radio_btn_off.png",
      "://gfx/radio_btn_on.png",
      "://gfx/radio_btn_on.png",
      styleByClassName ("crb_indicator"),
      styleByClassName ("crb_textstyle font16 darkblue normal lato"));

  rad->setStyleSheet (s);
}

/*-----------------------------------------*/
