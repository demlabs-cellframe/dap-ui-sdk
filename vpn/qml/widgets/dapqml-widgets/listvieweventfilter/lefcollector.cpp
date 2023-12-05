/* INCLUDES */

#include "lefcollector.h"

#include <QWheelEvent>
#include <QTimer>

/* DEFS */

#define DEBUGINFO qDebug() << (QString ("LefCollector::") + __func__)

//#define USE_PIXELDELTA

/*-----------------------------------------*/

#ifdef USE_PIXELDELTA
#define DELTA_VALUE pixelDelta
#else // USE_PIXELDELTA
#define DELTA_VALUE angleDelta
#endif // USE_PIXELDELTA

/*-----------------------------------------*/

struct LefCollector::Private
{
  /* VARS */
protected:
  QObject *target;
  QVector<Data> positions;
  QTimer *timeout;
  mutable QMutex mutex;

  /* CONSTRUCT/DESTRUCT */
public:
  Private();
  ~Private();

  /* METHODS */
public:
  void setTarget (QObject *a_target);
  void push (Data &&a_value);
  void push (const Data &a_value);
  int sum() const;
  void flush();
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LefCollector::LefCollector()
  : data (new Private)
{
  DEBUGINFO;
}

LefCollector::~LefCollector()
{
  delete data;
}

/********************************************
 * METHODS
 *******************************************/

void LefCollector::process (QObject *a_target, Data &a_data)
{
  /* store target */

  data->setTarget (a_target);

  /* check for macos mouse event */

  if (a_data.source == Qt::MouseEventSynthesizedBySystem
      && a_data.phase == Qt::ScrollUpdate)
  {
    /* store and set as handled */

    data->push (a_data);
    a_data.eventIsHandled = true;
  }
}

/*-----------------------------------------*/

LefCollector::Private::Private()
  : target (nullptr)
  , timeout (new QTimer)
{
  /* reserve slots */
  positions.reserve (128);

  /* setup flush timer */
  timeout->setSingleShot (true);
  timeout->setInterval (50);

  QObject::connect (timeout, &QTimer::timeout,
                    [this] { flush(); });
}

LefCollector::Private::~Private()
{
  timeout->stop();
  delete timeout;
}

void LefCollector::Private::setTarget (QObject *a_target)
{
  if (target == nullptr)
    target  = a_target;
}

void LefCollector::Private::push (Data &&a_value)
{
  DEBUGINFO;
  QMutexLocker l (&mutex);

  /* store */
  positions.append (std::move (a_value));

  /* start timer */
  if (!timeout->isActive())
    timeout->start();
}

void LefCollector::Private::push (const Data &a_value)
{
  DEBUGINFO;
  QMutexLocker l (&mutex);

  /* store */
  positions.append (a_value);

  /* start timer */
  if (!timeout->isActive())
    timeout->start();
}

int LefCollector::Private::sum() const
{
  int result = 0;

  for (const Data &value : positions)
    result += value.DELTA_VALUE.y();
  result /= positions.size();

  return result;
}

void LefCollector::Private::flush()
{
  DEBUGINFO;
  QMutexLocker l (&mutex);

  /* check */
  if (positions.isEmpty())
    return;

  /* get sum and clear positions */
  int result  = sum();
  DEBUGINFO << "result:" << result << "from" << positions.size() << "values";
  Data item = positions.first();
  positions.clear();

  /* construct fake event */
  QWheelEvent event(
    item.position,
    QPointF(),
    QPoint(),
    QPoint (0, result),
    Qt::NoButton,
    Qt::NoModifier,
    item.phase,
    item.inverted,
    item.source
  );

  /* send */
  QCoreApplication::sendEvent (target, &event);
}

/*-----------------------------------------*/
