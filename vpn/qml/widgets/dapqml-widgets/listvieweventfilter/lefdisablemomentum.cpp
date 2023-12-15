/* INCLUDES */

#include "lefdisablemomentum.h"

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("LefScrollFix::") + __func__)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LefDisableMomentum::LefDisableMomentum()
{
  DEBUGINFO;
}

LefDisableMomentum::~LefDisableMomentum()
{

}

/********************************************
 * METHODS
 *******************************************/

void LefDisableMomentum::process (QObject *a_target, Data &a_data)
{
  Q_UNUSED (a_target)

  /* skip momentum event */

  if (a_data.source == Qt::MouseEventSynthesizedBySystem
      && (
        a_data.phase == Qt::ScrollMomentum
        //|| a_data.phase == Qt::NoScrollPhase
        //|| a_data.phase == Qt::ScrollUpdate
        ))
    a_data.eventIsHandled = true;
}

/*-----------------------------------------*/
