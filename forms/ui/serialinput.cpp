/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"
#include "kelguilineedit.h"
#include <QDebug>

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)

/* VARS */
static SerialInput *__inst          = nullptr;
static int s_oldSerialLength  = 0;

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

  /* fill map */
  m_input = ui->btnSerial->edit(); //ui->editSerial;

#ifdef Q_OS_ANDROID
  m_input->setCallbackFocusEvent (cbSerialFocus);
  m_input->setCallbackTextEdit (cbSerialText);
#else
  ui->btnSerial->setInputMask (">NNNN-NNNN-NNNN-NNNN;_");
#endif // Q_OS_ANDROID

  /* signals */

////  connect (m_input, &KelGuiLineEdit::textChanged,
////           this, &SerialInput::slotTextChanged);
////  connect (m_input, &KelGuiLineEdit::textEdited,
////           this, &SerialInput::slotTextChanged);

//  connect (m_input, &KelGuiLineEdit::sigTextEditing,
//           this, &SerialInput::slotTextChanged,
//           Qt::QueuedConnection);
//  connect (m_input, &KelGuiLineEdit::editingFinished,
//           this, &SerialInput::slotCloseInput,
//           Qt::QueuedConnection);
//  connect (m_input, &KelGuiLineEdit::returnPressed,
//           this, &SerialInput::slotCloseInput,
//           Qt::QueuedConnection);

  connect (this, &SerialInput::sigReturn,
           this, &SerialInput::slotCloseInput,
           Qt::QueuedConnection);

  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::slotTextChanged,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::sigConfirm,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
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
  return m_input->text();
}

void SerialInput::cbSerialFocus(KelGuiLineEdit *e, const Qt::FocusReason &reason)
{
  Q_UNUSED (e)

  if (reason == Qt::MouseFocusReason)
    {
      auto btn  = __inst->ui->btnSerial;
      btn->setMainText ("");
    }
}

void SerialInput::cbSerialText(KelGuiLineEdit *e, QString &preedit, QString &commit, int from, int to)
{
  /* compare length's */
  //s_oldSerialLength = e->text().length();
  int len     = (preedit.length() > commit.length())
                ? preedit.length()
                : commit.length();
  auto &text  = (preedit.length() > commit.length())
                ? preedit
                : commit;
//  if (from || to)
//    {
//      s_enteredSerial.replace (e->selectionStart() + from, to, commit);
//    }

  /* fix strings */
  fixSerialString (e, preedit, true);
  fixSerialString (e, commit, true);

  /* loose focus */
  if (len == EXPECT_LENGTH && s_oldSerialLength == EXPECT_LENGTH)
    {
      __inst->ui->btnSerial->setFocus (Qt::MouseFocusReason);
      //__inst->ui->btnSerial->setMainText (text);
      qDebug() << "finished with text:" << text;
    }

  qDebug() << __PRETTY_FUNCTION__
           << "old" << s_oldSerialLength
           << "len" << len
           << "commit" << commit
           << "preedit" << preedit
           << "text" << e->text();

  /* update old */
  s_oldSerialLength = len;
}

void SerialInput::fixSerialString(KelGuiLineEdit *e, QString &serial, bool inserted)
{
  /* get copy */
  auto text   = serial.toUpper();

  /* fix letters */
  for (auto i = text.begin(), e = text.end(); i != e; i++)
    if ((*i < 0x30)
        || ((*i > 0x39) && (*i < 0x41))
        || (*i > 0x5A))
      *i = 'A';

  if (inserted)
    {
      /* add separator */
      if (text.length() == 4
          || text.length() == 9
          || text.length() == 14)
        {
          text += '-';
          //e->cursorForward (false);
          //e->setCursorPosition (text.length());
        }

      /* insert separator */
      if (text.length() >= 4 && text.at (4) != '-')
        text.insert (4, '-');
      if (text.length() >= 9 && text.at (9) != '-')
        text.insert (9, '-');
      if (text.length() >= 14 && text.at (14) != '-')
        text.insert (14, '-');
    }

//  /* replace separators */
//  if (text.length() >= 4)
//    text.replace (4, 1, '-');
//  if (text.length() >= 9)
//    text.replace (9, 1, '-');
//  if (text.length() >= 14)
//    text.replace (14, 1, '-');

  /* check if limit reachced */
  if (text.length() < MAX_LENGTH)
    {
      if (!text.isEmpty())
        serial  = text;
      return;
    }
  else
    {
      /* make loose focus */
      QEvent event (QEvent::FocusOut);
      QApplication::sendEvent (e, &event);
    }

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  serial    = text;
}

/********************************************
 * SLOTS
 *******************************************/

void SerialInput::slotTextChanged()
{
  if(m_textChangeHook)
    return;

  //qDebug() << __PRETTY_FUNCTION__ << "start";

  /* get input text */
  auto text   = m_input->text().toUpper();

  /* add separators */
  for(int i = 0; i < text.length(); i++)
    {
      /* AAAA-AAAA-AAAA-AAAA */
      /*     4    9    E     */

      /* insert separators */
      //qDebug() << __PRETTY_FUNCTION__ << "sep";
      if ((i == 4) || (i == 9) || (i == 14))
        {
          if (i == 4)
            if (text.at(4) != '-')
              text.insert(4,'-');
          if (i == 9)
            if (text.at(9) != '-')
              text.insert(9,'-');
          if (i == 14)
            if (text.at(14) != '-')
              text.insert(14,'-');
        }

      /* replace wrong characters */
      //qDebug() << __PRETTY_FUNCTION__ << "wr";
      QChar c = text.at(i);
      if (
          (
            c < 0x30

            || (c > 0x39
            && c < 0x41)

            || c > 0x5A
          )

          && c != '-')
        text.replace (i, 1, "_");
    }

  m_textChangeHook = true;

  /* check if limit reachced */
  if(text.length() < MAX_LENGTH)
    {
      //qDebug() << __PRETTY_FUNCTION__ << "skippo";
      if (!text.isEmpty())
        m_input->setText (text);

      //qDebug() << __PRETTY_FUNCTION__ << "skipped" << text;
      m_textChangeHook = false;
      return;
    }

  //qDebug() << __PRETTY_FUNCTION__ << "finno";
  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  m_input->setText (text);

  /* fix cursor pos */
  m_input->setCursorPosition (diff);

  qDebug() << __PRETTY_FUNCTION__ << "result" << text;
  m_textChangeHook = false;
}

void SerialInput::slotSetSerial(const QString &a_serial)
{
  if(m_textChangeHook)
    return;

  m_textChangeHook = true;
  qDebug() << __PRETTY_FUNCTION__ << "start";

  m_input->setText (a_serial);

  qDebug() << __PRETTY_FUNCTION__ << "finish";
  m_textChangeHook = false;
}

void SerialInput::slotCloseInput()
{
  QGuiApplication::inputMethod()->hide();
}

/*-----------------------------------------*/
