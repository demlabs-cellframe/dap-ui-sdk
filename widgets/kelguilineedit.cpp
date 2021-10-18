/* INCLUDES */
#include "kelguilineedit.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

bool KelGuiLineEdit::hideAnchor() const
{
  return m_hideAnchor;
}

void KelGuiLineEdit::setHideAnchor(bool newHideAnchor)
{
  m_hideAnchor = newHideAnchor;
}

KelGuiLineEdit::KelGuiLineEdit (QWidget *parent)
  : QLineEdit (parent)
  , m_callbackTextEdit (nullptr)
  , m_callbackFocusEvent (nullptr)
  , m_callbackKeyEvent (nullptr)
{

}

/********************************************
 * METHODS
 *******************************************/

KelGuiLineEdit::cbTextEdit KelGuiLineEdit::callbackTextEdit() const
{
  return m_callbackTextEdit;
}

void KelGuiLineEdit::setCallbackTextEdit(KelGuiLineEdit::cbTextEdit cb)
{
  m_callbackTextEdit = cb;
}


KelGuiLineEdit::cbFocusEvent KelGuiLineEdit::callbackFocusEvent() const
{
  return m_callbackFocusEvent;
}

void KelGuiLineEdit::setCallbackFocusEvent(KelGuiLineEdit::cbFocusEvent newCallbackFocusEvent)
{
  m_callbackFocusEvent = newCallbackFocusEvent;
}

KelGuiLineEdit::cbKeyEvent KelGuiLineEdit::callbackKeyEvent() const
{
  return m_callbackKeyEvent;
}

void KelGuiLineEdit::setCallbackKeyEvent(KelGuiLineEdit::cbKeyEvent newCallbackKeyEvent)
{
  m_callbackKeyEvent = newCallbackKeyEvent;
}

void KelGuiLineEdit::slotUnfocus()
{
  QEvent event (QEvent::FocusOut);
  QGuiApplication::sendEvent (this, &event);
  QGuiApplication::inputMethod()->hide();
  emit textChanged (text());
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiLineEdit::keyPressEvent(QKeyEvent *event)
{
  qDebug() << __PRETTY_FUNCTION__
           << "key:" << event->key()
           << "text:" << event->text();

  if (m_callbackKeyEvent)
    if (m_callbackKeyEvent (this, event))
      return;

  QLineEdit::keyPressEvent (event);
}

#ifdef Q_OS_ANDROID

void KelGuiLineEdit::inputMethodEvent(QInputMethodEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__
           << "commitString:" << e->commitString()
           << "preeditString:" << e->preeditString()
           << "replacement:" << e->replacementStart()
           << "-" << e->replacementLength();

//  /* ctl input max length */
//  if(e->preeditString().length() == this->maxLength())
//  {
//      /* make loose focus */
//      QEvent event(QEvent::FocusOut);
//      QApplication::sendEvent(this, &event);
//  }

  /* get entered strings */
  QString preedit = e->preeditString();
  QString commit  = e->commitString();

  /* callback */
  if (m_callbackTextEdit /*&& !e->replacementLength()*/)
    m_callbackTextEdit (this, preedit, commit, e->replacementStart(), e->replacementLength());

  /* simulate event */
  QInputMethodEvent newEvent (preedit, e->attributes());
  newEvent.setCommitString (
    commit,
    e->replacementStart(),
    e->replacementLength());
  QLineEdit::inputMethodEvent (&newEvent); //(e);

  /* send notify */
  emit sigTextEditing();
}

void KelGuiLineEdit::focusInEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

//  if(e->reason() == Qt::MouseFocusReason)
//      clear();
  if (m_callbackFocusEvent)
    m_callbackFocusEvent (this, e->reason());

  QLineEdit::focusInEvent(e);
}

QVariant KelGuiLineEdit::inputMethodQuery(Qt::InputMethodQuery a_imq) const
{
  if (m_hideAnchor)
    return inputMethodQuery (a_imq, QVariant());
  return QLineEdit::inputMethodQuery (a_imq);
}

QVariant KelGuiLineEdit::inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const
{
  /* if any options set */
  if (m_hideAnchor)
    {
      switch (property)
        {

        case Qt::ImAnchorRectangle:
        case Qt::ImCursorRectangle:
          /* ignore anchor & cursor */
          return QRect (-1024, -1024, 0, 0);

        default: break;

        }
    }

  return QLineEdit::inputMethodQuery (property, argument);
}

#endif // Q_OS_ANDROID

/*-----------------------------------------*/
