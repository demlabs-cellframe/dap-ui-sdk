/* INCLUDES */
#include "dapqmlswitchctl.h"

#include <QVariant>
#include <QMetaMethod>
#include <QDebug>

/* DEFS */
#define ENABLE_PRINTS

struct DapQmlSwitchCtl::DapQmlSwitchCtlData
{
  struct
  {
    QObject *root;
    QObject *background;
    QObject *toggle;
    QObject *toggleAnim;
    QObject *touchingPoint;
    QObject *touchingArea;
  } item;

  bool dragging;
  bool draggingAnim;
  bool draggingState;
  qreal pos1;
  qreal pos2;
  qreal diff;
  qreal draggingStartDistance;
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlSwitchCtl::DapQmlSwitchCtl()
  : _data (new DapQmlSwitchCtlData)
{

}

DapQmlSwitchCtl::~DapQmlSwitchCtl()
{
  delete _data;
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlSwitchCtl::setRoot (QObject *a_value)
{
  _data->item.root  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.root = nullptr; });
}

void DapQmlSwitchCtl::setBackground (QObject *a_value)
{
  _data->item.background  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.background = nullptr; });
}

void DapQmlSwitchCtl::setToggle (QObject *a_value)
{
  _data->item.toggle  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.toggle = nullptr; });
}

void DapQmlSwitchCtl::setToggleAnimation (QObject *a_value)
{
  _data->item.toggleAnim  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.toggleAnim = nullptr; });
}

void DapQmlSwitchCtl::setTouchingPoint (QObject *a_value)
{
  _data->item.touchingPoint  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.touchingPoint = nullptr; });
  connect (a_value, SIGNAL (xChanged()),
           this, SLOT (_slotTouchingPointXChanged()));
  connect (a_value, SIGNAL (startXChanged()),
           this, SLOT (_slotTouchAreaPressed()));
}

void printSignalList (const QMetaObject* metaObject)
{
  int methodCount = metaObject->methodCount();
  for (int i = metaObject->methodOffset(); i < methodCount; ++i)
  {
    QMetaMethod method = metaObject->method (i);
    if (method.methodType() == QMetaMethod::Signal)
      qDebug() << "Signal:" << method.methodSignature();
  }
}

void DapQmlSwitchCtl::setTouchArea (QObject *a_value)
{
  _data->item.touchingArea  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _data->item.touchingArea = nullptr; });

  //printSignalList (a_value->metaObject());

//  connect (a_value, SIGNAL (pressed(QList<QObject*>)),
//           this, SLOT (_slotTouchAreaPressed()));
  connect (a_value, SIGNAL (released(QList<QObject*>)),
           this, SLOT (_slotTouchAreaReleased()));
}

bool DapQmlSwitchCtl::dragging() const
{
  return _data->dragging;
}

void DapQmlSwitchCtl::setDragging (bool a_value)
{
  if (_data->dragging == a_value)
    return;

  _data->dragging = a_value;
  emit sigDraggingChanged();
}

bool DapQmlSwitchCtl::draggingAnim() const
{
  return _data->draggingAnim;
}

void DapQmlSwitchCtl::setDraggingAnim (bool a_value)
{
  if (_data->draggingAnim == a_value)
    return;

  _data->draggingAnim = a_value;
  emit sigDraggingAnimChanged();
}

bool DapQmlSwitchCtl::draggingState() const
{
  return _data->draggingState;
}

void DapQmlSwitchCtl::setDraggingState (bool a_value)
{
  if (_data->draggingState == a_value)
    return;

  _data->draggingState = a_value;
  emit sigDraggingStateChanged();
}

qreal DapQmlSwitchCtl::pos1() const
{
  return _data->pos1;
}

void DapQmlSwitchCtl::setPos1 (qreal a_value)
{
  if (_data->pos1 == a_value)
    return;

  _data->pos1 = a_value;
  emit sigPos1Changed();
}

qreal DapQmlSwitchCtl::pos2() const
{
  return _data->pos2;
}

void DapQmlSwitchCtl::setPos2 (qreal a_value)
{
  if (_data->pos2 == a_value)
    return;

  _data->pos2 = a_value;
  emit sigPos2Changed();
}

qreal DapQmlSwitchCtl::diff() const
{
  return _data->diff;
}

void DapQmlSwitchCtl::setDiff (qreal a_value)
{
  if (_data->diff == a_value)
    return;

  _data->diff = a_value;
  emit sigDiffChanged();
}

qreal DapQmlSwitchCtl::draggingStartDistance() const
{
  return _data->draggingStartDistance;
}

void DapQmlSwitchCtl::setDraggingStartDistance (qreal a_value)
{
  if (_data->draggingStartDistance == a_value)
    return;

  _data->draggingStartDistance = a_value;
  emit sigDraggingStartDistanceChanged();
}

bool DapQmlSwitchCtl::isReady()
{
  return
    _data->item.root
    && _data->item.background
    && _data->item.toggle
    && _data->item.toggleAnim
    && _data->item.touchingPoint
    && _data->item.touchingArea
    ;
}

template<class T>
T itemValue (QObject *a_item, const char *a_field, T a_default);

template<>
qreal itemValue (QObject *a_item, const char *a_field, qreal a_default)
{
  if (a_item == nullptr)
    return a_default;
  return a_item->property (a_field).toDouble();
}

template<>
bool itemValue (QObject *a_item, const char *a_field, bool a_default)
{
  if (a_item == nullptr)
    return a_default;
  return a_item->property (a_field).toBool();
}

void DapQmlSwitchCtl::_begin()
{
  auto x  = itemValue<qreal> (_data->item.touchingPoint, "x", 0);
  setPos1 (x);
  setPos2 (x);
  _updateDiff();
  _updateTglState();
  setDragging (false);
  setDraggingAnim (false);

  _print ("drag begin");
}

void DapQmlSwitchCtl::_end()
{
  /* turn off anim */
  setDraggingAnim (false);

  /* not dragging */
  if (!dragging())
    _toggle();
  else
  {
    /* on left side */
    if (!draggingState())
      _turnOff();
    /* on right side */
    else
      _turnOn();
  }

  /* finish */
  setDragging (false);

  _print ("drag end");
}

void DapQmlSwitchCtl::_move()
{
  /* lambda's */
  auto enableDragging = [this]
  {
    setDragging (true);
    setDraggingAnim (true);
  };

  /* store value */
  setPos2 (itemValue<qreal> (_data->item.touchingPoint, "x", pos2()));

  /* check for dragging */
  if (diff() > 0)
  {
    if (diff() > draggingStartDistance())
      enableDragging();
  }
  else
    if (diff() < 0)
    {
      if (diff() < -draggingStartDistance())
        enableDragging();
    }

  _updateDiff();
  _updateTglState();
  _print ("drag move");
}

void DapQmlSwitchCtl::_turnOff()
{
  bool checked  = itemValue<bool> (_data->item.root, "checked", false);

  if (checked == false)
    return;

  QMetaObject::invokeMethod (_data->item.root, "setState", Q_ARG(QVariant,false));

  _print ("turnOff");
}

void DapQmlSwitchCtl::_turnOn()
{
  bool checked  = itemValue<bool> (_data->item.root, "checked", true);

  if (checked == true)
    return;

  QMetaObject::invokeMethod (_data->item.root, "setState", Q_ARG(QVariant,true));

  _print ("turnOff");
}

void DapQmlSwitchCtl::_toggle()
{
  QMetaObject::invokeMethod (_data->item.root, "toggle");
}

void DapQmlSwitchCtl::_updateDiff()
{
  setDiff (pos2() - pos1());
}

void DapQmlSwitchCtl::_updateTglState()
{
  setDraggingState (itemValue<bool> (_data->item.toggle, "draggingState", draggingState()));
}

void DapQmlSwitchCtl::_print (const char *a_text)
{
#ifndef ENABLE_PRINTS
  Q_UNUSED (a_text);
#else // ENABLE_PRINTS
  qDebug ("DapQmlSwitchCtl %s: %.2f|%.2f|%.2f|%d|%d",
          a_text, pos1(), pos2(), diff(), dragging(), draggingState());
#endif // ENABLE_PRINTS
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlSwitchCtl::_slotTouchingPointXChanged()
{
  _move();
}

void DapQmlSwitchCtl::_slotTouchAreaPressed()
{
  _begin();
}

void DapQmlSwitchCtl::_slotTouchAreaReleased()
{
  _end();
}

/*-----------------------------------------*/
