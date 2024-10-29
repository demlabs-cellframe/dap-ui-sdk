/* INCLUDES */

#include "dapqmllistvieweventfilter.h"
#include "listvieweventfilter/lefbase.h"
#include "listvieweventfilter/leftest.h"
#include "listvieweventfilter/lefcollector.h"
#include "listvieweventfilter/lefscrollfix.h"
#include "listvieweventfilter/lefdisablemomentum.h"

#include <QTimer>
#include <QMutex>

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("DapQmlListviewEventFilter::") + __func__)

//#define ENABLE_EVENT_PRINTS

struct DapQmlListviewEventFilter::Data
{
  QObject *attachTarget = nullptr;
  QVector<LefBase*> lefChain;
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlListviewEventFilter::DapQmlListviewEventFilter (QQuickItem *a_parent)
  : QQuickItem (a_parent)
  , data (new Data)
{
  /* fill lef chain */

  data->lefChain
    //<< new LefTest
    << new LefDisableMomentum
    << new LefScrollFix
    //<< new LefCollector
  ;
}

DapQmlListviewEventFilter::~DapQmlListviewEventFilter()
{
  delete data;
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlListviewEventFilter::attachFilter (QObject *a_object)
{
  if (a_object == nullptr)
    return;

  /* create event filter */
  DapQmlListviewEventFilter *filter = new DapQmlListviewEventFilter;

  /* store target */
  filter->data->attachTarget  = a_object;

  /* install filter */
  a_object->installEventFilter (filter);
}

bool DapQmlListviewEventFilter::eventFilter (QObject *a_watched, QEvent *a_event)
{
  /* handle mouse wheel event */
  if (a_event->type() == QEvent::Wheel)
    {
      /* get event and print its content */
      QWheelEvent *wheelEvent = static_cast<QWheelEvent *> (a_event);

#ifdef ENABLE_EVENT_PRINTS
      DEBUGINFO << "pos:" << wheelEvent->position()
//                << "globalPos:" << wheelEvent->globalPosition()
                << "pixelDelta:" << wheelEvent->pixelDelta()
                << "angleDelta:" << wheelEvent->angleDelta()
//                << "buttons:" << wheelEvent->buttons()
//                << "modifiers:" << wheelEvent->modifiers()
                << "phase:" << wheelEvent->phase()
                << "inverted:" << wheelEvent->inverted()
                << "source:" << wheelEvent->source()
        ;
#endif // ENABLE_EVENT_PRINTS

      /* create usable copy of wheel event */
      LefBase::Data eventData
      {
        wheelEvent->position(),
        wheelEvent->globalPosition(),
        wheelEvent->pixelDelta(),
        wheelEvent->angleDelta(),
        wheelEvent->buttons(),
        wheelEvent->modifiers(),
        wheelEvent->phase(),
        wheelEvent->inverted(),
        wheelEvent->source(),
        false
      };

      /* push into chain */
      for (auto *lef : qAsConst (data->lefChain))
      {
        /* process only, if no one marked event as handled */
        if (!eventData.eventIsHandled)
          lef->process (data->attachTarget, eventData);

        /* if chain item marked as handled, do not pass this event back */
        else
          return true;
      }
  }

  /* done */
  return QQuickItem::eventFilter (a_watched, a_event);
}

/*-----------------------------------------*/
