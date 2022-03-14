/* INCLUDES */
#include "dapguilineedit.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

bool DapGuiLineEdit::hideAnchor() const
{
  return m_hideAnchor;
}

void DapGuiLineEdit::setHideAnchor(bool newHideAnchor)
{
  m_hideAnchor = newHideAnchor;
}

DapGuiLineEdit::DapGuiLineEdit (QWidget *parent)
#ifdef USE_QLABEL
  : QLabel(parent)
  , m_callbackEvent(nullptr)
#else
  : QLineEdit (parent)
#endif
  , m_callbackTextEdit (nullptr)
  , m_callbackFocusEvent (nullptr)
  , m_callbackKeyEvent (nullptr)
#ifdef USE_QLABEL
  , commiting("")
#endif
{

}

/********************************************
 * METHODS
 *******************************************/

DapGuiLineEdit::cbTextEdit DapGuiLineEdit::callbackTextEdit() const
{
  return m_callbackTextEdit;
}

void DapGuiLineEdit::setCallbackTextEdit(DapGuiLineEdit::cbTextEdit cb)
{
  m_callbackTextEdit = cb;
}


DapGuiLineEdit::cbFocusEvent DapGuiLineEdit::callbackFocusEvent() const
{
  return m_callbackFocusEvent;
}

void DapGuiLineEdit::setCallbackFocusEvent(DapGuiLineEdit::cbFocusEvent newCallbackFocusEvent)
{
  m_callbackFocusEvent = newCallbackFocusEvent;
}

DapGuiLineEdit::cbKeyEvent DapGuiLineEdit::callbackKeyEvent() const
{
  return m_callbackKeyEvent;
}

void DapGuiLineEdit::setCallbackKeyEvent(DapGuiLineEdit::cbKeyEvent newCallbackKeyEvent)
{
  m_callbackKeyEvent = newCallbackKeyEvent;
}

void DapGuiLineEdit::slotUnfocus()
{
  QEvent event (QEvent::FocusOut);
  QGuiApplication::sendEvent (this, &event);
  QGuiApplication::inputMethod()->hide();
  emit textChanged (text());
}

/********************************************
 * OVERRIDE
 *******************************************/

#ifndef USE_QLABEL
void DapGuiLineEdit::keyPressEvent(QKeyEvent *event)
{
  qDebug() << __PRETTY_FUNCTION__
           << "key:" << event->key()
           << "text:" << event->text();

  if (m_callbackKeyEvent)
    if (m_callbackKeyEvent (this, event))
      return;

  QLineEdit::keyPressEvent (event);
}
#endif

#ifdef USE_QLABEL

void DapGuiLineEdit::setText(QString a_text)
{
    QLabel::setText(a_text);
    emit textChanged(a_text);
}

void DapGuiLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (m_callbackEvent)
        if (m_callbackEvent (this, (QEvent*) event))
            return;
    QLabel::keyPressEvent(event);
}

void DapGuiLineEdit::leaveEvent(QEvent * event)
{
    if (m_callbackEvent)
        if (m_callbackEvent (this, (QEvent*) event))
            return;
    QLabel::leaveEvent(event);
}

void DapGuiLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_callbackEvent)
        if (m_callbackEvent (this, (QEvent*) event))
            return;
    QLabel::mouseReleaseEvent(event);
}

void DapGuiLineEdit::inputMethodEvent(QInputMethodEvent *event)
{
    if (m_callbackEvent)
        if (m_callbackEvent (this, (QEvent*) event))
            return;
    QLabel::inputMethodEvent(event);
}

void DapGuiLineEdit::showEvent(QShowEvent * event)
{
    if (m_callbackEvent)
        if (m_callbackEvent (this, (QEvent*) event))
            return;
    QLabel::showEvent(event);
}

#endif

void DapGuiLineEdit::setCallbackEvent(DapGuiLineEdit::cbInputMethodEvent cb)
{
   m_callbackEvent = cb;
}

DapGuiLineEdit::cbInputMethodEvent DapGuiLineEdit::callbackEvent() const
{
    return m_callbackEvent;
}

#ifdef Q_OS_ANDROID_UNUSED

void DapGuiLineEdit::inputMethodEvent(QInputMethodEvent *e)
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

void DapGuiLineEdit::focusInEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();

//  if(e->reason() == Qt::MouseFocusReason)
//      clear();
  if (m_callbackFocusEvent)
    m_callbackFocusEvent (this, e->reason());

  QLineEdit::focusInEvent(e);
}

QVariant DapGuiLineEdit::inputMethodQuery(Qt::InputMethodQuery a_imq) const
{
  if (m_hideAnchor)
    return inputMethodQuery (a_imq, QVariant());
  return QLineEdit::inputMethodQuery (a_imq);
}

QVariant DapGuiLineEdit::inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const
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
