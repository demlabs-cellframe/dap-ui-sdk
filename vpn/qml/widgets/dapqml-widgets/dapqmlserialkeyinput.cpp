/* INCLUDES */
#include "dapqmlserialkeyinput.h"
#include <QDebug>
#include <QApplication>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlSerialKeyInput::DapQmlSerialKeyInput(QObject *a_parent)
  : m_callbackEvent (nullptr)
  , m_qmlItem (nullptr)
{
  Q_UNUSED (a_parent)
  //setAcceptTouchEvents (true);
}

/********************************************
 * METHODS
 *******************************************/

const QString &DapQmlSerialKeyInput::commiting() const
{
  return m_commiting;
}

void DapQmlSerialKeyInput::setCommiting(const QString &newCommiting)
{
  m_commiting = newCommiting;
}

QString DapQmlSerialKeyInput::text() const
{
  if (m_qmlItem == nullptr)
    return QString();
  return m_qmlItem->property("text").toString();
}

void DapQmlSerialKeyInput::setText(const QString &a_text)
{
  if (m_qmlItem == nullptr)
    return;
  m_qmlItem->setProperty ("text", a_text);
  emit textChanged();
}

DapQmlSerialKeyInput::cbInputMethodEvent DapQmlSerialKeyInput::callbackEvent() const
{
  return m_callbackEvent;
}

void DapQmlSerialKeyInput::setCallbackEvent(cbInputMethodEvent newCallbackInputMethodEvent)
{
  m_callbackEvent = newCallbackInputMethodEvent;
}

void DapQmlSerialKeyInput::setup (QObject *a_qmlItem)
{
  /* store */
  m_qmlItem = a_qmlItem;

  if (a_qmlItem)
    {
      /* setup filter */
      a_qmlItem->installEventFilter (this);

      /* connect signal on deletion */
      connect (a_qmlItem, &QObject::destroyed, [=] { m_qmlItem = nullptr; } );
    }
}

void DapQmlSerialKeyInput::setFocus()
{
  QMetaObject::invokeMethod (m_qmlItem, "forceActiveFocus");
}

bool DapQmlSerialKeyInput::sendBackspaceEvent()
{
  if (m_qmlItem == nullptr)
    return true;

  QKeyEvent event (QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
  QApplication::sendEvent(m_qmlItem, &event);

  return false;
}

bool DapQmlSerialKeyInput::eventFilter(QObject *watched, QEvent *event)
{
  Q_UNUSED(watched)
  if (m_callbackEvent)
      if (!m_callbackEvent (this, (QEvent*) event))
          return true;
  return false; //QQuickItem::event (event);
}

/*-----------------------------------------*/
