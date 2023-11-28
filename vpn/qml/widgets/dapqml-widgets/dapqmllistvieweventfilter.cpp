/* INCLUDES */

#include "dapqmllistvieweventfilter.h"

#include <QTimer>
#include <QMutex>

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("DapQmlListviewEventFilter::") + __func__)
#define ARRAYSIZE(a) (sizeof(a) / sizeof(*(a)))

// enable this to start simulating MacOS scrolling
//#define ENABLE_FAKE

/**
 * @brief Class used to collect positions and sum the result
 */
class WheelEventCollection
{
  /* VARS */
protected:
  QObject *_target;
  QVector<int> _positions;
  QTimer *_timeout;
  mutable QMutex _mutex;

  /* CONSTRUCT/DESTRUCT */
public:
  WheelEventCollection (QObject *a_target);
  ~WheelEventCollection();

  /* METHODS */
public:
  void push (int a_value);
  int sum() const;
  void flush();
};

struct DapQmlListviewEventFilter::Data
{
  /* VARS */

#ifdef ENABLE_FAKE
  QTimer *fakeEventTimer          = nullptr;
  unsigned long fakeEventCounter  = 0;
  QWheelEvent *fakeEvent          = nullptr;
#endif // ENABLE_FAKE

  QObject *attachTarget             = nullptr;
  WheelEventCollection *collection  = nullptr;

  /* CONSTRUCT/DESTRUCT */

  Data()
  {
#ifdef ENABLE_FAKE
    fakeEventTimer  = new QTimer;
    fakeEventTimer->setSingleShot (false);
    fakeEventTimer->setInterval (10);
#endif // ENABLE_FAKE
  }

  ~Data()
  {
#ifdef ENABLE_FAKE
    delete fakeEventTimer;
    delete fakeEvent;
#endif // ENABLE_FAKE
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlListviewEventFilter::DapQmlListviewEventFilter (QQuickItem *a_parent)
  : QQuickItem (a_parent)
  , data (new Data)
{
#ifdef ENABLE_FAKE
  connect (data->fakeEventTimer, &QTimer::timeout,
           this, &DapQmlListviewEventFilter::_slotFakeEvent);
#endif // ENABLE_FAKE
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

  /* creaste event filter */
  DapQmlListviewEventFilter *filter = new DapQmlListviewEventFilter;

  /* store target */
  filter->data->attachTarget  = a_object;

  /* create event collection and attach taregt */
  filter->data->collection    = new WheelEventCollection (a_object);

  /* install filter */
  a_object->installEventFilter (filter);
}

bool DapQmlListviewEventFilter::eventFilter (QObject *a_watched, QEvent *a_event)
{
#ifdef ENABLE_FAKE

  /* handle mouse wheel event */
  if (a_event->type() == QEvent::Wheel)
    {
      QWheelEvent *wheelEvent = static_cast<QWheelEvent *> (a_event);
      DEBUGINFO << "pos:" << wheelEvent->position()
//                << "globalPos:" << wheelEvent->globalPosition()
//                << "pixelDelta:" << wheelEvent->pixelDelta()
                << "angleDelta:" << wheelEvent->angleDelta()
//                << "buttons:" << wheelEvent->buttons()
//                << "modifiers:" << wheelEvent->modifiers()
                << "phase:" << wheelEvent->phase()
                << "inverted:" << wheelEvent->inverted()
                << "source:" << wheelEvent->source()
        ;

      /* start fakes */
      if (data->fakeEvent == nullptr)
      {
        data->fakeEvent = new QWheelEvent (*wheelEvent);
        data->fakeEventTimer->start();
        return true;
      }
    }

#else // ENABLE_FAKE

  /* handle mouse wheel event */
  if (a_event->type() == QEvent::Wheel)
    {
      QWheelEvent *wheelEvent = static_cast<QWheelEvent *> (a_event);
      DEBUGINFO << "pos:" << wheelEvent->position()
//                << "globalPos:" << wheelEvent->globalPosition()
//                << "pixelDelta:" << wheelEvent->pixelDelta()
                << "angleDelta:" << wheelEvent->angleDelta()
//                << "buttons:" << wheelEvent->buttons()
//                << "modifiers:" << wheelEvent->modifiers()
                << "phase:" << wheelEvent->phase()
                << "inverted:" << wheelEvent->inverted()
                << "source:" << wheelEvent->source()
        ;

      /* check for bad event */
      if (wheelEvent->source() != Qt::MouseEventNotSynthesized)
      //if (wheelEvent->position().x() != 0)
        {
          //QWheelEvent(QPointF pos, QPointF globalPos, QPoint pixelDelta, QPoint angleDelta,
          //            Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::ScrollPhase phase,
          //            bool inverted, Qt::MouseEventSource source = Qt::MouseEventNotSynthesized);

          /* get move speed */
          auto yPos = wheelEvent->angleDelta().y();

          if (yPos != 0)
          {
//            /* construct proper event */
//            QWheelEvent event(
//                QPointF(),
//                QPointF(),
//                QPoint(),
//                QPoint (0, yPos / 16),
//                wheelEvent->buttons(),
//                wheelEvent->modifiers(),
//                wheelEvent->phase(),
//                wheelEvent->inverted(),
//                Qt::MouseEventNotSynthesized
//              );

//            /* send */
//            QCoreApplication::sendEvent (data->attachTarget, &event);

            /* store value */
            DEBUGINFO;

            data->collection->push (yPos);

            return true;
          }
      }
  }

#endif // ENABLE_FAKE

  /* done */
  return QQuickItem::eventFilter (a_watched, a_event);
}

/*-----------------------------------------*/

#ifdef ENABLE_FAKE
const int s_values[] =
{
  -2,
  -4,
  -6,
  -8,
  -8,
  -8,
  -8,
  -8,
  -8,
  -8,
  -10,
  -32,
  -10,
  -8,
  -10,
  -10,
  -10,
  -12,
  -12,
  -12,
  -10,
  -12,
  -14,
  -14,
  -18,
  -18,
  -18,
  -18,
  -16,
  -12,
  -10,
  -8,
  -8,
  0,
  0,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -20,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  0,
  0,
  -2,
  -2,
  -6,
  -6,
  -6,
  -10,
  -10,
  -10,
  -12,
  -10,
  -12,
  -12,
  -12,
  -12,
  -12,
  -14,
  -16,
  -16,
  -16,
  -14,
  -14,
  -12,
  -14,
  -12,
  -10,
  -10,
  -8,
  -6,
  -6,
  -8,
  -10,
  -10,
  -10,
  -10,
  0,
  0,
  -10,
  -10,
  -10,
  -10,
  -10,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  0,
  0,
  -2,
  -6,
  -6,
  -6,
  -8,
  -8,
  -8,
  -10,
  -8,
  -10,
  -10,
  -12,
  -10,
  -10,
  -10,
  -10,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -14,
  -12,
  -16,
  -18,
  -18,
  -16,
  0,
  0,
  -18,
  -20,
  -20,
  -20,
  -20,
  -18,
  -20,
  -20,
  -18,
  -16,
  -16,
  -14,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  0,
  0,
  0,
  0,
  -2,
  -2,
  -8,
  -8,
  -10,
  -12,
  -10,
  -10,
  -12,
  -10,
  -12,
  -12,
  -12,
  -12,
  -14,
  -14,
  -16,
  -16,
  -16,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -8,
  0,
  0,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  -2,
  0,
  0,
  0,
  0,
  -2,
  -2,
  -4,
  -8,
  -10,
  -12,
  -12,
  -14,
  -12,
  -10,
  -10,
  -8,
  -4,
  -2,
  -2,
  -2,
  0,
  0,
  0,
  2,
  2,
  4,
  8,
  10,
  10,
  10,
  10,
  8,
  8,
  10,
  8,
  12,
  14,
  16,
  18,
  24,
  24,
  28,
  30,
  0,
  0,
  32,
  34,
  36,
  36,
  34,
  32,
  36,
  34,
  32,
  28,
  26,
  24,
  22,
  20,
  18,
  18,
  16,
  14,
  14,
  12,
  12,
  10,
  10,
  10,
  8,
  8,
  8,
  6,
  6,
  6,
  6,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  2,
  6,
  12,
  10,
  10,
  12,
  12,
  12,
  12,
  12,
  14,
  14,
  14,
  16,
  0,
  0,
  16,
  18,
  18,
  18,
  16,
  16,
  18,
  16,
  16,
  14,
  12,
  12,
  10,
  10,
  10,
  8,
  8,
  8,
  0,
  0,
  -2,
  -4,
  -6,
  -6,
  -8,
  -10,
  -12,
  -14,
  -16,
  -52,
  -18,
  -16,
  -20,
  -16,
  -16,
  -16,
  -16,
  -18,
  -20,
  0,
  0,
  -24,
  -26,
  -26,
  -26,
  -26,
  -54,
  -26,
  -26,
  -22,
  -20,
  -18,
  -18,
  -16,
  -14,
  -14,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  0,
  0,
  -2,
  -2,
  0,
  -6,
  -6,
  -6,
  -8,
  -10,
  -8,
  -8,
  -8,
  -8,
  -8,
  -10,
  -12,
  -14,
  -14,
  -14,
  -14,
  -12,
  -10,
  -12,
  -10,
  -10,
  -8,
  -10,
  0,
  0,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  -2,
  -2,
  -2,
  0,
  0,
  -2,
  -4,
  -8,
  -8,
  -12,
  -12,
  -12,
  -12,
  -10,
  -8,
  -10,
  -8,
  -8,
  -10,
  -12,
  -14,
  -16,
  -16,
  -12,
  -10,
  -10,
  -10,
  -10,
  -10,
  -8,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  -8,
  -6,
  -6,
  -6,
  -4,
  -4,
  -4,
  -4,
  -4,
  -2,
  -2,
  0,
  0,
  -2,
  -8,
  -10,
  -18,
  -24,
  -20,
  -30,
  -22,
  -22,
  -30,
  -28,
  -32,
  -34,
  -36,
  -36,
  -34,
  -32,
  -32,
  0,
  0,
  -46,
  -52,
  -52,
  -52,
  -52,
  -50,
  -56,
  -54,
  -48,
  -44,
  -40,
  -38,
  -34,
  -32,
  0,
  0,
  2,
  6,
  10,
  12,
  12,
  14,
  14,
  20,
  18,
  22,
  24,
  22,
  30,
  28,
  0,
  0,
  40,
  42,
  44,
  44,
  44,
  40,
  46,
  44,
  40,
  38,
  34,
  32,
  28,
  26,
  24,
  22,
  20,
  18,
  16,
  16,
  14,
  12,
  12,
  10,
  10,
  10,
  8,
  8,
  8,
  12,
  6,
  6,
  4,
  4,
  0,
  0,
  2,
  6,
  10,
  12,
  14,
  16,
  16,
  18,
  18,
  20,
  18,
  26,
  28,
  30,
  32,
  28,
  0,
  0,
  42,
  46,
  46,
  48,
  46,
  46,
  50,
  48,
  44,
  40,
  38,
  34,
  32,
  28,
  26,
  24,
  22,
  20,
  18,
  16,
  16,
  14,
  14,
  12,
  10,
  10,
  10,
  10,
  16,
  8,
  6,
  6,
  6,
  4,
  0,
  0,
  2,
  2,
  10,
  10,
  12,
  14,
  16,
  16,
  16,
  16,
  16,
  18,
  24,
  20,
  28,
  30,
  32,
  36,
  38,
  36,
  38,
  52,
  58,
  58,
  58,
  56,
  56,
  60,
  60,
  54,
  48,
  44,
  40,
  38,
  34,
  32,
  28,
  26,
  24,
  22,
  20,
  18,
  16,
  0,
  0,
  -2,
  -4,
  -8,
  -10,
  -14,
  -16,
  -22,
  -24,
  -52,
  -26,
  -30,
  -22,
  -30,
  -24,
  -28,
  -26,
  -32,
  0,
  0,
  -44,
  -48,
  -48,
  -48,
  -92,
  -50,
  -48,
  -44,
  -40,
  -38,
  -34,
  -32,
  -28,
  -26,
  -24,
  -22,
  -20,
  -18,
  -16,
  -16,
  -14,
  -14,
  -12,
  -10,
  -10,
  0,
  0,
  -2,
  -2,
  0,
  -6,
  -8,
  -10,
  -12,
  -14,
  -16,
  -16,
  -18,
  -20,
  -22,
  -24,
  -26,
  -22,
  -30,
  -26,
  -26,
  -30,
  -20,
  -22,
  0,
  0,
  -30,
  -34,
  -34,
  -36,
  -34,
  -32,
  -36,
  -34,
  -32,
  -28,
  -26,
  -24,
  -22,
  -20,
  -18,
  -18,
  -16,
  -14,
  -14,
  -12,
  -12,
  -10,
  -10,
  -10,
  -8,
  -8,
  0,
  0,
  -2,
  -8,
  -10,
  -10,
  -12,
  -12,
  -10,
  -8,
  -8,
  -8,
  -8,
  -8,
  -8,
  -8,
  -6,
  -4,
  -2,
  -2,
  0,
  0,
};
#endif // ENABLE_FAKE

/*-----------------------------------------*/

//QWheelEvent(QPointF pos, QPointF globalPos, QPoint pixelDelta, QPoint angleDelta,
//            Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::ScrollPhase phase,
//            bool inverted, Qt::MouseEventSource source = Qt::MouseEventNotSynthesized);

void DapQmlListviewEventFilter::_slotFakeEvent()
{
#ifdef ENABLE_FAKE
  /* check array boundaries */
  if (data->fakeEventCounter >= ARRAYSIZE(s_values))
    return data->fakeEventTimer->stop();

  /* construct fake event */
  QWheelEvent event(
    QPointF(),//fakeEvent->position(),
    QPointF(),//fakeEvent->globalPosition(),
    QPoint(),//fakeEvent->pixelDelta(),
    QPoint (0, s_values[data->fakeEventCounter++]),// fakeEvent->angleDelta(),
    data->fakeEvent->buttons(),
    data->fakeEvent->modifiers(),
    data->fakeEvent->phase(),
    data->fakeEvent->inverted(),
    Qt::MouseEventNotSynthesized//fakeEvent->source()
  );

  /* send */
  QCoreApplication::sendEvent (data->attachTarget, &event);
#endif // ENABLE_FAKE
}

/*-----------------------------------------*/

WheelEventCollection::WheelEventCollection (QObject *a_target)
  : _target (a_target)
  , _mutex (QMutex::Recursive)
{
  /* reserve slots */
  _positions.reserve (128);

  /* setup flush timer */
  _timeout  = new QTimer;
  _timeout->setSingleShot (true);
  _timeout->setInterval (50);

  QObject::connect (_timeout, &QTimer::timeout,
                    [this] { flush(); });
}

WheelEventCollection::~WheelEventCollection()
{
  _timeout->stop();
  delete _timeout;
}

void WheelEventCollection::push (int a_value)
{
  DEBUGINFO << a_value;
  QMutexLocker l (&_mutex);

  /* store */
  _positions.append (a_value);

  /* start timer */
  if (!_timeout->isActive())
    _timeout->start();
}

int WheelEventCollection::sum() const
{
  QMutexLocker l (&_mutex);

  int result = 0;

  for (int value : _positions)
    result += value;
  result /= _positions.size();

  return result;
}

void WheelEventCollection::flush()
{
  DEBUGINFO;
  QMutexLocker l (&_mutex);

  /* check */
  if (_positions.isEmpty())
    return;

  /* get sum and clear positions */
  int result  = sum();
  DEBUGINFO << "result:" << result << "from" << _positions.size() << "values";
  _positions.clear();

  /* construct fake event */
  QWheelEvent event(
    QPointF(),
    QPointF(),
    QPoint(),
    QPoint (0, result),
    Qt::NoButton,
    Qt::NoModifier,
    Qt::NoScrollPhase,
    false,
    Qt::MouseEventNotSynthesized
  );

  /* send */
  QCoreApplication::sendEvent (_target, &event);
}
