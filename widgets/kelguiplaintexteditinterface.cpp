/* INCLUDES */
#include "kelguiplaintexteditinterface.h"
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiPlainTextEditInterface::KelGuiPlainTextEditInterface (QWidget *parent)
  : QPlainTextEdit (parent)
{

}

/********************************************
 * METHODS
 *******************************************/

KelGuiPlainTextEditInterface::cbTextEdit KelGuiPlainTextEditInterface::callbackTextEdit() const
{
  return m_callbackTextEdit;
}

void KelGuiPlainTextEditInterface::setCallbackTextEdit(cbTextEdit cb)
{
  m_callbackTextEdit = cb;
}

KelGuiPlainTextEditInterface::cbFocusEvent KelGuiPlainTextEditInterface::callbackFocusEvent() const
{
  return m_callbackFocusEvent;
}

void KelGuiPlainTextEditInterface::setCallbackFocusEvent(cbFocusEvent newCallbackFocusEvent)
{
  m_callbackFocusEvent = newCallbackFocusEvent;
}

KelGuiPlainTextEditInterface::cbFocusEvent KelGuiPlainTextEditInterface::callbackUnfocusEvent() const
{
  return m_callbackUnfocusEvent;
}

void KelGuiPlainTextEditInterface::setCallbackUnfocusEvent(KelGuiPlainTextEditInterface::cbFocusEvent newCallbackUnfocusEvent)
{
  m_callbackUnfocusEvent = newCallbackUnfocusEvent;
}

KelGuiPlainTextEditInterface::cbKeyEvent KelGuiPlainTextEditInterface::callbackKeyEvent() const
{
  return m_callbackKeyEvent;
}

void KelGuiPlainTextEditInterface::setCallbackKeyEvent(cbKeyEvent newCallbackKeyEvent)
{
  m_callbackKeyEvent = newCallbackKeyEvent;
}

void KelGuiPlainTextEditInterface::keyPressEvent(QKeyEvent *event)
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

void KelGuiPlainTextEditInterface::inputMethodEvent(QInputMethodEvent *e)
{
  /* get entered strings */
  QString preedit = e->preeditString();
  QString commit  = e->commitString();

  qDebug() << __PRETTY_FUNCTION__ << "preedit:" << preedit << "commit" << commit;

  /* callback */
  if (m_callbackTextEdit /*&& !e->replacementLength()*/)
    m_callbackTextEdit (this, preedit, commit, e->replacementStart(), e->replacementLength());

  /* simulate event */
  QInputMethodEvent newEvent (preedit, e->attributes());
  newEvent.setCommitString (
    commit,
    e->replacementStart(),
    e->replacementLength());
  QPlainTextEdit::inputMethodEvent (&newEvent); //(e);
}

void KelGuiPlainTextEditInterface::focusInEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

  if (m_callbackFocusEvent)
    m_callbackFocusEvent (this, e->reason());

  QPlainTextEdit::focusInEvent(e);
}

void KelGuiPlainTextEditInterface::focusOutEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

  if (m_callbackUnfocusEvent)
    m_callbackUnfocusEvent (this, e->reason());

  QPlainTextEdit::focusOutEvent(e);
}

#endif // Q_OS_ANDROID

/*-----------------------------------------*/
