/* INCLUDES */
#include "dapqmlswitchctl.h"

#include <QVariant>
#include <QMetaMethod>
#include <QTimer>
#include <QDebug>

/* DEFS */
//#define ENABLE_PRINTS

class TogglePosCtl
{
  qreal finalPos;
  qreal draggingPos;
  qreal minPos;
  qreal maxPos;
  bool isLeftReached;
  bool isRightReached;
public:
  TogglePosCtl()
    : finalPos (0)
    , draggingPos (0)
    , minPos (0)
    , maxPos (0)
    , isLeftReached (false)
    , isRightReached (false)
  {

  }

  TogglePosCtl &evaluate(QObject *a_root, QObject *a_toggle, qreal a_pos);
  qreal getToggleX (bool a_dragging) const;
};

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

  TogglePosCtl togglePosCtl;

  DapQmlSwitchCtlData()
  {
    item.root          = nullptr;
    item.background    = nullptr;
    item.toggle        = nullptr;
    item.toggleAnim    = nullptr;
    item.touchingPoint = nullptr;
    item.touchingArea  = nullptr;

    dragging      = false;
    draggingAnim  = false;
    draggingState = false;
    pos1    = 0.0;
    pos2    = 0.0;
    diff    = 0.0;
    draggingStartDistance = 0.0;
  }
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlSwitchCtl::DapQmlSwitchCtl()
  : _data (new DapQmlSwitchCtlData)
{
  connect (this, &DapQmlSwitchCtl::sigDraggingChanged,
           this, &DapQmlSwitchCtl::_slotUpdateTogglePos);
  connect (this, &DapQmlSwitchCtl::sigPos2Changed,
           this, &DapQmlSwitchCtl::_slotUpdateTogglePos);
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
           this, [this] { _itemsCleared(); });
  connect (a_value, SIGNAL(checkedChanged()),
           this, SLOT(_slotStackUpdateTogglePos()));
  connect (a_value, SIGNAL(widthChanged()),
           this, SLOT(_slotUpdateTogglePos()));
}

void DapQmlSwitchCtl::setBackground (QObject *a_value)
{
  _data->item.background  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _itemsCleared(); });
}

void DapQmlSwitchCtl::setToggle (QObject *a_value)
{
  _data->item.toggle  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _itemsCleared(); });
  connect (a_value, SIGNAL(widthChanged()),
           this, SLOT(_slotUpdateTogglePos()));
}

void DapQmlSwitchCtl::setToggleAnimation (QObject *a_value)
{
  _data->item.toggleAnim  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _itemsCleared(); });
}

void DapQmlSwitchCtl::setTouchingPoint (QObject *a_value)
{
  QString objName = a_value->objectName();

  if (objName.isEmpty())
    return;

  _data->item.touchingPoint  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _itemsCleared(); });
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
  QString objName = a_value->objectName();

  if (objName.isEmpty())
    return;

  _data->item.touchingArea  = a_value;
  connect (a_value, &QObject::destroyed,
           this, [this] { _itemsCleared(); });

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

#ifdef ENABLE_PRINTS
  qDebug ("%s :: dragging %d", __PRETTY_FUNCTION__, _data->dragging);
#endif // ENABLE_PRINTS
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

  _print ("turnOn");
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
  //setDraggingState (itemValue<bool> (_data->item.toggle, "draggingState", draggingState()));
  qreal rootWidth = itemValue<qreal> (_data->item.root, "width", -1);
  if (rootWidth == -1)
    return;
  setDraggingState (pos2() >= rootWidth / 2);
}

void DapQmlSwitchCtl::_itemsCleared()
{
  _data->item.root          = nullptr;
  _data->item.background    = nullptr;
  _data->item.toggle        = nullptr;
  _data->item.toggleAnim    = nullptr;
  _data->item.touchingPoint = nullptr;
  _data->item.touchingArea  = nullptr;
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

void DapQmlSwitchCtl::_slotUpdateTogglePos()
{
  if (_data->item.root == nullptr
      || _data->item.toggle == nullptr)
    return;

  qreal toggleX = _data->togglePosCtl.evaluate(
    _data->item.root,
    _data->item.toggle,
    pos2()
  ).getToggleX (dragging());

  _data->item.toggle->setProperty ("x", toggleX);
}

void DapQmlSwitchCtl::_slotStackUpdateTogglePos()
{
  QTimer::singleShot (50,  this, [this] {_slotUpdateTogglePos();});
  QTimer::singleShot (100, this, [this] {_slotUpdateTogglePos();});
  QTimer::singleShot (150, this, [this] {_slotUpdateTogglePos();});
}

/*-----------------------------------------*/

TogglePosCtl &TogglePosCtl::evaluate (QObject *a_root, QObject *a_toggle, qreal a_pos)
{
  if (a_root == nullptr
      || a_toggle == nullptr)
    return *this;

  bool checked      = itemValue<bool> (a_root, "checked", false);
  qreal rootWidth   = itemValue<qreal> (a_root, "width", 0.0);
  qreal toggleWidth = itemValue<qreal> (a_toggle, "width", 0.0);

  minPos  = (-12.0 * (rootWidth / 270.0));
  maxPos  = (rootWidth - toggleWidth + 12.0 * (rootWidth / 270.0));

  finalPos    = checked ? maxPos : minPos;
  draggingPos = a_pos - (toggleWidth / 2.0);

  isLeftReached   = draggingPos <= minPos;
  isRightReached  = draggingPos >= maxPos;

#ifdef ENABLE_PRINTS
  qDebug ("%s :: checked %d, rootWidth %d, toggleWidth %d, minPos %d, maxPos %d, finalPos %d, draggingPos %d, isLeftReached %d, isRightReached %d",
    __PRETTY_FUNCTION__,
    checked,
    int (rootWidth),
    int (toggleWidth),
    int (minPos),
    int (maxPos),
    int (finalPos),
    int (draggingPos),
    isLeftReached,
    isRightReached);
#endif // ENABLE_PRINTS

  return *this;
}

qreal TogglePosCtl::getToggleX (bool a_dragging) const
{
#ifdef ENABLE_PRINTS
  qDebug ("%s :: dragging %d", __PRETTY_FUNCTION__, a_dragging);
#endif // ENABLE_PRINTS

  if (a_dragging)
  {
    if (isLeftReached)
      return minPos;
    if (isRightReached)
      return maxPos;
    return draggingPos;
  }

  return finalPos;
}

/*-----------------------------------------*/
