/* INCLUDES */
#include "dapguiplaintexteditinterface.h"
#include "dapguiplaintexteditstylemanager.h"
#include <QDebug>
#include <QApplication>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPlainTextEditInterface::DapGuiPlainTextEditInterface (QWidget *parent)
  : QPlainTextEdit (parent)
{

}

/********************************************
 * METHODS
 *******************************************/

DapGuiPlainTextEditInterface::cbTextEdit DapGuiPlainTextEditInterface::callbackTextEdit() const
{
  return m_callbackTextEdit;
}

void DapGuiPlainTextEditInterface::setCallbackTextEdit(cbTextEdit cb)
{
  m_callbackTextEdit = cb;
}

DapGuiPlainTextEditInterface::cbFocusEvent DapGuiPlainTextEditInterface::callbackFocusEvent() const
{
  return m_callbackFocusEvent;
}

void DapGuiPlainTextEditInterface::setCallbackFocusEvent(cbFocusEvent newCallbackFocusEvent)
{
  m_callbackFocusEvent = newCallbackFocusEvent;
}

DapGuiPlainTextEditInterface::cbFocusEvent DapGuiPlainTextEditInterface::callbackUnfocusEvent() const
{
  return m_callbackUnfocusEvent;
}

void DapGuiPlainTextEditInterface::setCallbackUnfocusEvent(DapGuiPlainTextEditInterface::cbFocusEvent newCallbackUnfocusEvent)
{
  m_callbackUnfocusEvent = newCallbackUnfocusEvent;
}

void DapGuiPlainTextEditInterface::unfocus()
{
  QEvent nve (QEvent::FocusOut);
  QApplication::sendEvent (this, &nve);
}

DapGuiPlainTextEditInterface::cbKeyEvent DapGuiPlainTextEditInterface::callbackKeyEvent() const
{
  return m_callbackKeyEvent;
}

void DapGuiPlainTextEditInterface::setCallbackKeyEvent(cbKeyEvent newCallbackKeyEvent)
{
  m_callbackKeyEvent = newCallbackKeyEvent;
}

void DapGuiPlainTextEditInterface::keyPressEvent(QKeyEvent *event)
{
  qDebug() << __PRETTY_FUNCTION__
           << "key:" << event->key()
           << "text:" << event->text();

  if (m_callbackKeyEvent)
    if (m_callbackKeyEvent (this, event))
      return;

  QPlainTextEdit::keyPressEvent (event);
}

#ifdef Q_OS_ANDROID

void DapGuiPlainTextEditInterface::inputMethodEvent(QInputMethodEvent *e)
{
  /* callback */
  if (m_callbackTextEdit /*&& !e->replacementLength()*/)
    {
      /* get entered strings */
      QString preedit = e->preeditString();
      QString commit  = e->commitString();
#ifdef QT_DEBUG
      qDebug() << __PRETTY_FUNCTION__ << "preedit:" << preedit << "commit" << commit;
#endif // QT_DEBUG
      if (m_callbackTextEdit (this, preedit, commit, e->replacementStart(), e->replacementLength()))
        {
          /* simulate event */
          QInputMethodEvent newEvent (preedit, e->attributes());
          newEvent.setCommitString (
            commit,
            e->replacementStart(),
            e->replacementLength());
          return QPlainTextEdit::inputMethodEvent (&newEvent);
        }
    }

  QPlainTextEdit::inputMethodEvent (e);
}

void DapGuiPlainTextEditInterface::focusInEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

  if (m_callbackFocusEvent)
    m_callbackFocusEvent (this, e->reason());

  QPlainTextEdit::focusInEvent(e);
}

void DapGuiPlainTextEditInterface::focusOutEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

  if (m_callbackUnfocusEvent)
    m_callbackUnfocusEvent (this, e->reason());

  QPlainTextEdit::focusOutEvent(e);
}

#endif // Q_OS_ANDROID

/*-----------------------------------------*/
