/* INCLUDES */

#include "lefscrollfix.h"

#include <QWheelEvent>

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("LefScrollFix::") + __func__)


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LefScrollFix::LefScrollFix()
{
  DEBUGINFO;
}

LefScrollFix::~LefScrollFix()
{

}

/********************************************
 * METHODS
 *******************************************/

void LefScrollFix::process (QObject *a_target, Data &a_data)
{
  /* check for macos event */

  auto pixelDelta = a_data.pixelDelta;

  if (a_data.source != Qt::MouseEventNotSynthesized
      && (pixelDelta.x() != 0 || pixelDelta.y() != 0))
    {
      /* construct fake event */

      QWheelEvent newEvent(
        a_data.position,
        a_data.globalPosition,
        QPoint(),//a_data.pixelDelta,
        a_data.pixelDelta,//a_data.angleDelta,
        a_data.buttons,
        a_data.modifiers,
        a_data.phase,
        a_data.inverted,
        a_data.source
      );

      /* send and set as handled */

      QCoreApplication::sendEvent (a_target, &newEvent);
      a_data.eventIsHandled = true;
    }
}

/*-----------------------------------------*/
