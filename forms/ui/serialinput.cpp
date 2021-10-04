/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"
#include "kelguilineedit.h"
#include <QDebug>

/* DEFS */
#define MAX_LENGTH (19)

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
  m_input = ui->editSerial;

  /* signals */
//  connect (m_input, &KelGuiLineEdit::textChanged,
//           this, &SerialInput::slotTextChanged);
//  connect (m_input, &KelGuiLineEdit::textEdited,
//           this, &SerialInput::slotTextChanged);
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

/********************************************
 * SLOTS
 *******************************************/

void SerialInput::slotTextChanged()
{
  if(m_textChangeHook)
    return;

  qDebug() << __PRETTY_FUNCTION__ << "start";

  /* get input text */
  auto text   = m_input->text().toUpper();

  /* add separators */
  for(int i = 0; i < text.length(); i++)
    {
      /* AAAA-AAAA-AAAA-AAAA */
      /*     4    9    E     */

      /* insert separators */
      qDebug() << __PRETTY_FUNCTION__ << "sep";
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
      qDebug() << __PRETTY_FUNCTION__ << "wr";
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
      qDebug() << __PRETTY_FUNCTION__ << "skippo";
      if (!text.isEmpty())
        m_input->setText (text);

      qDebug() << __PRETTY_FUNCTION__ << "skipped" << text;
      m_textChangeHook = false;
      return;
    }

  qDebug() << __PRETTY_FUNCTION__ << "finno";
  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  m_input->setText (text);

  /* fix cursor pos */
  m_input->setCursorPosition (diff);

  qDebug() << __PRETTY_FUNCTION__ << "finish" << text;
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

/*-----------------------------------------*/
