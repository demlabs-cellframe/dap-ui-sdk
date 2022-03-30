/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"
#include "dapguilineedit.h"
#include <QDebug>
#include <QKeyEvent>
#include <QClipboard>
#include <QMimeData>

#include "../forms/ui/helper/auxiliary/UiScaling.h"

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)
#define EMPTY_KEY_STR ("____ ____ ____ ____")

/* VARS */
static SerialInput *__inst          = nullptr;
QString SerialInput::keyFromBuffer  = "";

void labelFormatSerialKeyLine(QString &text)
{
    int textLength = text.length();
    if (textLength > 12)
        text.insert(12, "-");
    if (textLength > 8)
        text.insert(8, "-");
    if (textLength > 4)
        text.insert(4, "-");
}

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SerialInput::SerialInput(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SerialInput),
  m_textChangeHook (false)
{
  /* setup ui */
  __inst  = this;
  ui->setupUi(this);
  ui->editSerial->hide();
  ui->customInputBorder->hide();
//  ui->MainWidget->layout()->removeWidget (ui->customInputBorder);
//  ui->btnSerial->setEdit (ui->customInputBorder);
  {
    auto style = ui->btnSerial->edit()->cssStyle();
    style.remove ("noborder");
    style.append (" login-input-border");
    ui->btnSerial->edit()->setCssStyle(style);
  }

  /* store input ptr */
  m_input = ui->btnSerial->edit(); // ui->customInput;
  m_input->setText ("");
  m_input->setCallbackEvent(_cbInputMethodEvent);
  // m_input->setHideAnchor (true); // this will hide anchor for no reason
  m_input->setCallbackFocusEvent (cbFocusEvent);
  // m_input->setCallbackTextEdit (cbTextEdit); // this will mess the input
  m_input->setMaxLength (MAX_LENGTH);

  /* set font uppercase */
  {
    auto fontSize   = UiScaling::pointsToPixels (20, UiScaling::getNativDPI());
    QFont myFont ("Lato", fontSize);
    myFont.setCapitalization (QFont::AllUppercase);
    m_input->setFont (myFont);
  }

  /* signals */

  // notify about returning
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);

  // commit on any button
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           QGuiApplication::inputMethod(), &QInputMethod::commit,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &DapGuiPushButton::clicked,
           QGuiApplication::inputMethod(), &QInputMethod::commit,
           Qt::QueuedConnection);

  // notify about confirming,
  // return after confirming
  connect (ui->btnConfirm, &DapGuiPushButton::clicked,
           this, &SerialInput::sigConfirm,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &DapGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);

  // unfocus on any button
  connect (this, &SerialInput::sigReturn,
           m_input, &DapGuiLineEdit::slotUnfocus,
           Qt::QueuedConnection);
  connect (this, &SerialInput::sigConfirm,
           m_input, &DapGuiLineEdit::slotUnfocus,
           Qt::QueuedConnection);
}

SerialInput::~SerialInput()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

QString SerialInput::serialKey() const
{
  auto serial = m_input->text();
  fixSerialKey (serial);
  m_input->setHideAnchor (true);
  return serial;
}

void SerialInput::fixSerialKey(QString &a_serial) const
{
  /* get copy */
  auto text   = a_serial.toUpper();

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  a_serial    = text;

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
            continue;

        /* fix symbols */
        if ((k < QChar ('0' /*0x30*/))
            || ((k > QChar ('9' /*0x39*/)) && (k < QChar ('A' /*0x41*/)))
            || (k > QChar ('Z' /*0x5A*/)))
          {
            /* remove and restart cycle */
            text.remove (j, 1);
            goto repeat;
          }
        }

    {
      /* insert separator */
      if (text.length() >= 5 && text.at (4) != '-')
        text.insert (4, '-');
      if (text.length() >= 10 && text.at (9) != '-')
        text.insert (9, '-');
      if (text.length() >= 15 && text.at (14) != '-')
        text.insert (14, '-');
    }

  /* check if limit reachced */
  m_input->setText (text);
  if (text.length() < MAX_LENGTH)
    {
      a_serial  = text;
      return;
    }
}

void SerialInput::cbFocusEvent(DapGuiLineEdit *e, const Qt::FocusReason &reason)
{
  if (reason == Qt::MouseFocusReason)
    {
      e->setHideAnchor (false);
      if (e->text() == EMPTY_KEY_STR)
        e->setText("");
    }
}

void SerialInput::cbTextEdit(DapGuiLineEdit *e, QString &preedit, QString &commit, int from, int to)
{
  Q_UNUSED(from)
  Q_UNUSED(to)
  int len   = e->text().length() + preedit.length() + commit.length();
  if (len >= MAX_LENGTH)
    {
      QGuiApplication::inputMethod()->commit();
      e->slotUnfocus();
    }
}

/********************************************
 * SLOTS
 *******************************************/

void SerialInput::slotSetSerial(const QString &a_serial)
{
  QString empty = EMPTY_KEY_STR;
  if(m_textChangeHook)
    return;

  m_textChangeHook = true;
  if (a_serial.length() == 0)
    m_input->setText (empty);
  else
    m_input->setText (a_serial);
  m_textChangeHook = false;
}

void SerialInput::slotActiavted()
{
  m_input->setHideAnchor (false);
}

void SerialInput::slotRetranslated()
{
  ui->lTitle->setText (tr ("Serial Key"));
  ui->btnSerial->setSubText (tr ("insert serial key"));
  ui->btnConfirm->setText (tr ("CONFIRM"));
}


bool SerialInput::_cbInputMethodEvent(DapGuiLineEdit *label, QEvent *event)
{
    int keyMaxLen = EXPECT_LENGTH;
    QString empty = EMPTY_KEY_STR;

    if (event->type() == QEvent::InputMethod)
    {
        QInputMethodEvent *Event = (QInputMethodEvent *)(event);
        QString a = Event->preeditString();
        QString c = Event->commitString();

        if (a.length()==0 && c.length()==0)
            //label->clear();
            label->setText(empty);

        a = a.remove("-").toUpper();
        a = a.remove(QRegExp("[^A-Z0-9]"));
        c = c.remove("-").toUpper();
        c = c.remove(QRegExp("[^A-Z0-9]"));

        if (a.length()>0)
        {
           QString m = label->commiting + a;
           if (m.length() >= keyMaxLen)
           {
            m.remove(keyMaxLen, m.length() - keyMaxLen);
            QGuiApplication::inputMethod()->commit();
           }
           labelFormatSerialKeyLine(m);
           if (m == "")
               m = empty;
           label->setText(m);
           event->accept();
           return false;
        }

        if (c.length()>0)
        {
            label->commiting += c;
            QString t = label->commiting;
            if (label->commiting.length() > keyMaxLen)
            label->commiting.remove(keyMaxLen, label->commiting.length() - keyMaxLen);
            labelFormatSerialKeyLine(t);
            if (t == "")
                t = empty;
            label->setText(t);
            QGuiApplication::inputMethod()->commit();
            event->accept();
            return false;
        }
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *Event = (QKeyEvent *)(event);
        if (Event->key() == Qt::Key_Backspace)
        {
            if (label->commiting.length() == 0)
                //label->clear();
                label->setText(empty);
            else
            {
                label->commiting = label->commiting.remove(label->commiting.length()-1, 1);
                QString t  = label->commiting;
                labelFormatSerialKeyLine(t);
                if (t == "")
                    t = empty;
                label->setText(t);
                event->accept();
                return false;
            }
        }
        if (Event->key() == Qt::Key_Return)
        {
            QGuiApplication::inputMethod()->commit();
            if (label->commiting.length() == keyMaxLen)
                QGuiApplication::inputMethod()->hide();
        }
        if (Event->key() == Qt::Key_V && readFromBuffer())
         {
             label->commiting = keyFromBuffer;
             QString t = label->commiting;
             labelFormatSerialKeyLine(t);
             label->setText(t);
         }
    }

    if (event->type() == QEvent::Leave)
    {
        QGuiApplication::inputMethod()->hide();
    }

    if (event->type() == QEvent::Show)
    {
        if (label->commiting.length() == 0)
        {
            if (readFromBuffer())
            {
               label->commiting = keyFromBuffer;
               QString t = label->commiting;
               labelFormatSerialKeyLine(t);
               label->setText(t);
            }
            else
            {
                label->setText(empty);
            }
        }
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        return false;
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
//         QMouseEvent *Event = (QMouseEvent *)(event);
         label->setFocus();
         QGuiApplication::inputMethod()->update(Qt::ImHints);
         QGuiApplication::inputMethod()->show();
//         event->accept();
         return true;
    }
    return true;
}

bool SerialInput::readFromBuffer()
{
    int keyMaxLen = EXPECT_LENGTH;
    if( QClipboard* c = QApplication::clipboard() )
    {
        if( const QMimeData* m = c->mimeData() )
        {
//            if( m->hasText())
            {
                QString buffer = m->text();
                if (buffer.length() <= (keyMaxLen+3))
                {
                    buffer = buffer.remove("-").toUpper();
                    buffer = buffer.remove(QRegExp("[^A-Z0-9]"));
                    if (buffer.length() == keyMaxLen)
                    {
                        keyFromBuffer =  buffer;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
/*-----------------------------------------*/
