/* INCLUDES */
#include "dapguiradiostylemanager.h"
#include "dapguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiRadioStyleManager::DapGuiRadioStyleManager (QWidget *parent)
  : DapGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiRadioStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiRadioStyleManager::forcedRadioStyleUpdate);
    }
}

DapGuiRadioStyleManager::DapGuiRadioStyleManager (DapGuiRadioStyleManager &&src)
  : DapGuiStyleManager ((DapGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiRadioStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiRadioStyleManager::forcedRadioStyleUpdate);
    }
}

DapGuiRadioStyleManager::~DapGuiRadioStyleManager()
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void DapGuiRadioStyleManager::forcedRadioStyleUpdate()
{
  /* get push button ptr */
  DapGuiRadio *rad  = dynamic_cast<DapGuiRadio *> (m_widget);
  if (!rad)
    return;

  /* compile style */
  QString s =
    QString (
      "*\n{"
      "border: none;"
      "%5}\n"

      "QRadioButton{border: none;}\n"
      "DapGuiRadio{border: none;}\n"

      "QRadioButton::indicator\n{"
      "border: none;"
      "%1%4}\n"

      "QRadioButton::indicator:hover\n{"
      "border: none;"
      "%2}\n"

      "QRadioButton::indicator:checked\n{"
      "border: none;"
      "%3}\n")
    .arg (
      styleByClassName ("crb_off"),
      styleByClassName ("crb_on"),
      styleByClassName ("crb_on"),
      styleByClassName ("crb_indicator"),
      styleByClassName ("crb_textstyle font16 darkblue normal lato"));

  rad->setStyleSheet (s);
}

/*-----------------------------------------*/
