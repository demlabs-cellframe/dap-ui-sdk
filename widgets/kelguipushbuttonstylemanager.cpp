/* INCLUDES */
#include "kelguipushbuttonstylemanager.h"
#include "kelguipushbutton.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiPushButtonStyleManager::KelGuiPushButtonStyleManager (QWidget *parent)
  : KelGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiPushButtonStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiPushButtonStyleManager::forcedButtonStyleUpdate);
    }
}

KelGuiPushButtonStyleManager::KelGuiPushButtonStyleManager (KelGuiPushButtonStyleManager &&src)
  : KelGuiStyleManager ((KelGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
                  this, &KelGuiPushButtonStyleManager::forcedStyleUpdate);
      connect (&_signal(), &KelGuiStyleManager::forceStyleUpdate,
               this, &KelGuiPushButtonStyleManager::forcedButtonStyleUpdate);
    }
}

KelGuiPushButtonStyleManager::~KelGuiPushButtonStyleManager()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void KelGuiPushButtonStyleManager::setCssStyle (const QString &cssStyle)
{
  m_cssStyle  = cssStyle;
  forcedButtonStyleUpdate();
}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void KelGuiPushButtonStyleManager::forcedButtonStyleUpdate()
{
  /* get push button ptr */
  KelGuiPushButton *pb  = dynamic_cast<KelGuiPushButton *> (m_widget);
  if (!pb)
    return;

  /* collect 3 files name + style */
  QString names[4] =
  {
    tr (":/") + pb->custom().path(),
    tr (":/") + pb->customHover().path(),
    tr (":/") + pb->customPushed().path(),
    pb->customCss(),
  };
  auto objName = pb->objectName();

  QString s =
    QString (
      "*\n{"
      "border-image: url(\"%1\");%4}\n"

      "*:hover\n{"
      "border-image: url(\"%2\");}\n"

      "*:checked\n{"
      "border-image: url(\"%3\");}\n"

      "QPushButton\n{"
      "color: #FFFFFF;"
      "%5"
      "font-weight: bold;"
      "font-style: bold;"
      "font-size: bold;"
      "text-transform: uppercase;"
      "font-family: 'Roboto';"
      "background-color: rgba(0,0,0,0);}\n")
    .arg (
      names[0],
      names[1],
      names[2],
      styleByClassName (names[3]),
      styleByClassName ("cw_pushbutton"));

  pb->setStyleSheet (s);
}

/*-----------------------------------------*/
