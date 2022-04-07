/* INCLUDES */
#include "dapqmlserialkeyinput.h"
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlSerialKeyInput::DapQmlSerialKeyInput(QObject *a_parent)
  : QQuickItem()
  , m_callbackEvent (nullptr)
{
  Q_UNUSED (a_parent)
}

/********************************************
 * METHODS
 *******************************************/

DapQmlSerialKeyInput::cbInputMethodEvent DapQmlSerialKeyInput::callbackEvent() const
{
  return m_callbackEvent;
}

void DapQmlSerialKeyInput::setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent)
{
  m_callbackEvent = newCallbackInputMethodEvent;
}

void DapQmlSerialKeyInput::touchEvent(QTouchEvent *event)
{
  if (m_callbackEvent)
      if (m_callbackEvent (this, (QEvent*) event))
          return;
  return QQuickItem::touchEvent (event);
}

void DapQmlSerialKeyInput::keyPressEvent(QKeyEvent *event)
{
  if (m_callbackEvent)
      if (m_callbackEvent (this, (QEvent*) event))
          return;
  return QQuickItem::keyPressEvent (event);
}

void DapQmlSerialKeyInput::mouseReleaseEvent(QMouseEvent *event)
{
  if (m_callbackEvent)
      if (m_callbackEvent (this, (QEvent*) event))
          return;
  return QQuickItem::mouseReleaseEvent (event);
}

void DapQmlSerialKeyInput::mousePressEvent(QMouseEvent *event)
{
  if (m_callbackEvent)
      if (m_callbackEvent (this, (QEvent*) event))
          return;
  return QQuickItem::mousePressEvent (event);
}

void DapQmlSerialKeyInput::inputMethodEvent(QInputMethodEvent *event)
{
  if (m_callbackEvent)
      if (m_callbackEvent (this, (QEvent*) event))
          return;
  return QQuickItem::inputMethodEvent (event);
}

//bool DapQmlSerialKeyInput::event(QEvent *event)
//{
//  if (m_callbackEvent)
//      if (m_callbackEvent (this, (QEvent*) event))
//          return;
//  return QQuickItem::event (event);
//}

/*-----------------------------------------*/
