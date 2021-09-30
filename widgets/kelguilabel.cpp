/* INCLUDES */
#include "kelguilabel.h"
#include "kelguicommon.h"

#include <QMutex>
#include <QMap>
#include <QMouseEvent>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiLabel::KelGuiLabel (QWidget *parent)
  : QLabel (parent)
  , _cssHook (false)
{
  connect (&__kgsm, &KelGuiStyleManager::cssStyleChanged,
           this, &KelGuiLabel::_slotCssStyleUpdate);
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiLabel::mousePressEvent (QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    emit clicked();
}

/********************************************
 * SLOTS
 *******************************************/

void KelGuiLabel::_slotCssStyleUpdate()
{
  if(_cssHook)
    {
      _cssHook = false;
      return;
    }

  _cssHook = true;
  __kgsm.setCssStyle (__kgsm.cssStyle() + " noborder nobackground");
}

/*-----------------------------------------*/
