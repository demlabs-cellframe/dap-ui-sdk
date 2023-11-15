/* INCLUDES */

#include "dapqmllistvieweventfilter.h"

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("DapQmlListviewEventFilter::") + __func__)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlListviewEventFilter::DapQmlListviewEventFilter (QQuickItem *a_parent)
  : QQuickItem (a_parent)
{

}

DapQmlListviewEventFilter::~DapQmlListviewEventFilter()
{

}

/********************************************
 * METHODS
 *******************************************/

void DapQmlListviewEventFilter::attachFilter (QObject *a_object)
{
  if (a_object == nullptr)
    return;

  a_object->installEventFilter (new DapQmlListviewEventFilter);
}

bool DapQmlListviewEventFilter::eventFilter (QObject *a_watched, QEvent *a_event)
{
  /* handle mouse wheel event */
  if (a_event->type() == QEvent::Wheel)
    {
      QWheelEvent *wheelEvent = static_cast<QWheelEvent *> (a_event);
      DEBUGINFO << "source:" << wheelEvent->source()
                << "angleDelta:" << wheelEvent->angleDelta();
    }

  /* done */
  return QQuickItem::eventFilter (a_watched, a_event);
}

/*-----------------------------------------*/
