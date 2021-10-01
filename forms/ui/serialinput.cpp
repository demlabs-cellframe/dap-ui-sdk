/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"

/* DEFS */
#define MAX_LENGTH (4)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

SerialInput::SerialInput(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SerialInput),
  m_textChangeHook (false)
{
  /* setup ui */
  ui->setupUi(this);

  /* fill map */
  m_inputMap =
  {
    {ui->editSerial1, 0},
    {ui->editSerial2, 1},
    {ui->editSerial3, 2},
    {ui->editSerial4, 3},
  };

  /* signals */
  for (auto i = m_inputMap.begin(), e = m_inputMap.end(); i != e; i++)
    {
      auto input = qobject_cast<KelGuiPlainTextEdit*> (i.key());

      if (!input)
        continue;

      connect (input, &KelGuiPlainTextEdit::textChanged,
               this, &SerialInput::slotTextChanged);
    }

  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::sigConfirm,
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
  QStringList result;

  for (auto i = m_inputMap.begin(), e = m_inputMap.end(); i != e; i++)
    {
      auto input = qobject_cast<KelGuiPlainTextEdit*> (i.key());

      if (!input)
        continue;

      result += input->plainText();
    }

  return result.join("-");
}

/********************************************
 * SLOTS
 *******************************************/

void SerialInput::slotTextChanged()
{
  if(m_textChangeHook)
    return;

  /* get input and id */
  auto input  = qobject_cast<KelGuiPlainTextEdit*> (sender());
  auto id     = m_inputMap.value (input, -1);
  if (id == -1)
    return;

  /* vars */
  auto text = input->plainText();

  /* check if limit reachced */
  if(text.length() < MAX_LENGTH)
    return;

  m_textChangeHook = true;

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  input->setPlainText (text);

  /* fix cursor pos */
  auto cur  = input->textCursor();
  cur.movePosition (QTextCursor::End, QTextCursor::MoveAnchor);
  input->setTextCursor (cur);

  /* move focus to next input */
  if (id < 3)
    {
      auto next = qobject_cast<KelGuiPlainTextEdit*> (m_inputMap.key(id + 1));
      if (next)
        {
          next->editWidget()->setFocus();
        }
    }
  m_textChangeHook = false;
}

void SerialInput::slotSetSerial(const QString &a_serial)
{
  QStringList list = a_serial.split("-");

  m_textChangeHook = true;

  for (auto i = m_inputMap.begin(), e = m_inputMap.end(); i != e; i++)
    {
      auto input = qobject_cast<KelGuiPlainTextEdit*> (i.key());

      if (!input)
        continue;

      input->setPlainText ((!list.isEmpty()) ? (list.takeFirst()) : (""));
    }

  m_textChangeHook = false;
}

/*-----------------------------------------*/
