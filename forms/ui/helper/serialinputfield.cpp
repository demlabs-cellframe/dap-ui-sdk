/* INCLUDES */
#include "serialinputfield.h"
#include <QDebug>
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QFocusEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QTextDocument>
#include <QKeySequence>
#include <QClipboard>

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SerialInputField::SerialInputField(QWidget *parent)
  : QWidget(parent)
  , m_child (new QWidget(this))
  , m_text ("____-____-____-____")
  , m_temp (m_text)
  , m_cursor (0)
  , m_focused (false)
{
  m_child->resize (1, 1);
  m_child->hide();
  setAttribute (Qt::WA_InputMethodEnabled);
  setFocusPolicy (Qt::StrongFocus);
}

/********************************************
 * SLOTS
 *******************************************/

void SerialInputField::slotCursorPositionChanged(int oldPos, int newPos)
{
  qDebug() << __PRETTY_FUNCTION__
           << "oldPos:" << oldPos
           << "newPos:" << newPos;
}

void SerialInputField::slotFixString()
{
  /* get copy */
  auto text   = m_temp.toUpper();

  /* fix letters */
repeat:
    /* start counter */
    int j = 0;

    /* run thru cycle */
    for (auto i = text.cbegin(), e = text.cend(); i != e; i++, j++)
      {
        QChar k = *i;

        /* ignore minus */
        if ((k == '-')
            && (j == 4 || j == 9 || j == 14))
          {
            //qDebug() << "skipping" << k << j;
            continue;
          }

        /* fix symbols */
        if ((k < QChar ('0' /*0x30*/))
            || ((k > QChar ('9' /*0x39*/)) && (k < QChar ('A' /*0x41*/)))
            || (k > QChar ('Z' /*0x5A*/)))
          {
            //*i = '_';
            /* remove and restart cycle */
            //qDebug() << "removing" << k << j;
            text.remove (j, 1);
            goto repeat;
          }
        //else
          //qDebug() << k << j << *reinterpret_cast<ushort*> (&k);
        }

    {
//      /* add separator */
//      if (text.length() == 4
//          || text.length() == 9
//          || text.length() == 14)
//          text += '-';

      /* insert separator */
      if (text.length() >= 5 && text.at (4) != '-')
        text.insert (4, '-');
      if (text.length() >= 10 && text.at (9) != '-')
        text.insert (9, '-');
      if (text.length() >= 15 && text.at (14) != '-')
        text.insert (14, '-');
    }

  /* check if limit reachced */
  if (text.length() < MAX_LENGTH)
    {
      m_temp  = text;
      return;
    }
//  else
//    {
//      /* make loose focus */
//      QEvent event (QEvent::FocusOut);
//      QGuiApplication::sendEvent (this, &event);
//    }

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  m_temp    = text;
}

void SerialInputField::slotUnfocus()
{
  QEvent event (QEvent::FocusOut);
  QGuiApplication::sendEvent (this, &event);
  QGuiApplication::inputMethod()->hide();
//  m_child->show();
//  m_child->setFocus (Qt::MouseFocusReason);
//  m_child->hide();
}

/********************************************
 * OVERRIDE
 *******************************************/

void SerialInputField::mousePressEvent(QMouseEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__
           << "pos:" << e->pos()
           << "windowPos:" << e->windowPos();
  QWidget::mousePressEvent (e);

  /* force focus in */
  if (!m_focused)
    {
      QFocusEvent in (QEvent::FocusIn, Qt::MouseFocusReason);
      QGuiApplication::sendEvent (this, &in);
      setFocus (Qt::MouseFocusReason);
    }
}

void SerialInputField::keyPressEvent(QKeyEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__
           << "key:" << e->key()
           << "text:" << e->text();

  /* on paste */
  if (e == QKeySequence::Paste)
    {
      /* disable event */
      e->ignore();

      /* get clipboard and fix content */
      auto text = QGuiApplication::clipboard()->text().toUpper();
      m_temp    = text;
      slotFixString ();

      /* store result and unfocus */
      m_text    = m_temp;
      slotUnfocus();

      qDebug() << __PRETTY_FUNCTION__
               << "pasted:" << text
               << "result:" << m_text;
      return;
    }

  QWidget::keyPressEvent (e);
}

void SerialInputField::inputMethodEvent(QInputMethodEvent *e)
{
  /* get text */
  auto commit   = e->commitString().toUpper();
  auto preedit  = e->preeditString().toUpper();
  auto text     = commit + preedit;
  auto oldCur   = m_cursor;

  /* fix serial */
  m_temp  = text;
  slotFixString();
  m_cursor      = m_temp.length();
  {
    QString tt = "____-____-____-____";
    tt.replace (0, m_temp.length(), m_temp);
    m_temp  = tt;
  }

  qDebug() << __FUNCTION__
           << "commitString:" << e->commitString()
           << "preeditString:" << e->preeditString()
           << "replacement:" << e->replacementStart()
           << "-" << e->replacementLength()
           << "cursor" << m_cursor
           << "text" << text
           << "temp" << m_temp;

  /* commit */
  if (!commit.isEmpty() || m_cursor == MAX_LENGTH)
    {
      m_text  = m_temp;
      slotUnfocus();
    }
//  else
//    QGuiApplication::inputMethod()->commit();

  /* finish */
  if (oldCur != m_cursor)
    emit cursorPositionChanged (oldCur, m_cursor);
  e->accept();
  repaint();
}

void SerialInputField::focusInEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();
  m_focused = true;
  m_text  = "____-____-____-____";
  m_temp  = "____-____-____-____";
  QGuiApplication::inputMethod()->show();
  repaint();
}

void SerialInputField::focusOutEvent(QFocusEvent *e)
{
  qDebug() << __PRETTY_FUNCTION__ << "reason:" << e->reason();
  m_focused = false;
  QGuiApplication::inputMethod()->hide();
  repaint();
}

void SerialInputField::paintEvent(QPaintEvent *)
{
  qDebug() << __PRETTY_FUNCTION__ << m_temp;

  /* start painter */
  QPainter p (this);

  /* vars */
  auto rich   = m_temp;
  auto cur    = m_cursor;

  /* adjust cursor */
  if (cur == 4)
    cur++;
  else if (cur == 9)
    cur++;
  else if (cur == 14)
    cur++;

  /* format text */
  if (cur < MAX_LENGTH && m_focused)
    {
      auto c = rich.at (cur);
      auto l = QString ("<font color='#DA0B82'>%1</font>").arg (c);
      rich.replace (cur, 1, l);
    }
  rich  = "<center style='position: absolute; bottom: 0px;'>" + rich + "</center>";

  /* draw text */
//  p.drawText (rect(), Qt::AlignCenter, rich);

  QTextDocument doc;
  QTextOption opt  = doc.defaultTextOption();
  opt.setAlignment (Qt::AlignHCenter | Qt::AlignVCenter);
  doc.setDefaultTextOption (opt);
  doc.setTextWidth (rect().width());
  doc.setHtml (rich);

  doc.drawContents (&p, rect());
}

QVariant SerialInputField::inputMethodQuery (Qt::InputMethodQuery a_imq) const
{
  return inputMethodQuery (a_imq, QVariant());
}

QVariant SerialInputField::inputMethodQuery(Qt::InputMethodQuery property, QVariant argument) const
{
  Q_UNUSED (argument);
//  qDebug() << __PRETTY_FUNCTION__
//           << "property:" << property
//           << "argument:" << argument;

  switch(property)
    {
    case Qt::ImCursorRectangle:
      return QRect (2, 2, 1, 32);//d->cursorRect();
//    case Qt::ImAnchorRectangle:
//        return d->adjustedControlRect(d->control->anchorRect());
    case Qt::ImFont:
      return font();
    case Qt::ImCursorPosition: {
      return QRect (2, 2, 1, 32);
//        const QPointF pt = argument.toPointF();
//        if (!pt.isNull())
//            return QVariant(d->xToPos(pt.x(), QTextLine::CursorBetweenCharacters));
//        return QVariant(d->control->cursor());
      }
    case Qt::ImSurroundingText:
      return m_text;
    case Qt::ImCurrentSelection:
      return QVariant();
    case Qt::ImMaximumTextLength:
      return EXPECT_LENGTH;
    case Qt::ImAnchorPosition:
//        if (d->control->selectionStart() == d->control->selectionEnd())
//            return QVariant(d->control->cursor());
//        else if (d->control->selectionStart() == d->control->cursor())
//            return QVariant(d->control->selectionEnd());
//        else
//            return QVariant(d->control->selectionStart());
      return QRect (2, 2, 1, 32);
    default:
        return QWidget::inputMethodQuery(property);
    }
}

/*-----------------------------------------*/
