/* INCLUDES */
#include "dapguilabel.h"
#include "dapguicommon.h"

#include <QMutex>
#include <QMap>
#include <QMouseEvent>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiLabel::DapGuiLabel (QWidget *parent)
  : QLabel (parent)
  , _cssHook (false)
{
  connect (&__kgsm, &DapGuiStyleManager::cssStyleChanged,
           this, &DapGuiLabel::_slotCssStyleUpdate);
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiLabel::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
}

/********************************************
 * SLOTS
 *******************************************/

void DapGuiLabel::_slotCssStyleUpdate()
{
  if(_cssHook)
    {
      _cssHook = false;
      return;
    }

  _cssHook = true;

  /* transparent attach */
  auto css = __kgsm.cssStyle();
  if (!css.contains ("noborder") && !css.contains ("nobackground"))
    __kgsm.setCssStyle (__kgsm.cssStyle() + " noborder nobackground");
}

/*-----------------------------------------*/
