/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"
#include "kelguilineedit.h"
#include <QDebug>
#include <QKeyEvent>
#include <QClipboard>

#include "../forms/ui/helper/auxiliary/UiScaling.h"

/* DEFS */
#define EXPECT_LENGTH (16)
#define MAX_LENGTH (EXPECT_LENGTH + 3)

/* VARS */
static SerialInput *__inst          = nullptr;

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
  //m_input->setHideAnchor (true);
  m_input->setCallbackFocusEvent (cbFocusEvent);
  m_input->setMaxLength (MAX_LENGTH);

  /* set font uppercase */
  {
    auto fontSize   = UiScaling::pointsToPixels (20, UiScaling::getNativDPI());
    QFont myFont ("Lato", fontSize);
    myFont.setCapitalization (QFont::AllUppercase);
    m_input->setFont (myFont);
  }

  /* signals */

  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);

  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::sigConfirm,
           Qt::QueuedConnection);
  connect (ui->btnConfirm, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
           Qt::QueuedConnection);

  connect (this, &SerialInput::sigReturn,
           m_input, &KelGuiLineEdit::slotUnfocus,
           Qt::QueuedConnection);
  connect (this, &SerialInput::sigConfirm,
           m_input, &KelGuiLineEdit::slotUnfocus,
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

void SerialInput::cbFocusEvent(KelGuiLineEdit *e, const Qt::FocusReason &reason)
{
  if (reason == Qt::MouseFocusReason)
    {
      e->setHideAnchor (false);
      if (e->text() == "____-____-____-____")
        e->setText ("");
    }
}

/********************************************
 * SLOTS
 *******************************************/

void SerialInput::slotSetSerial(const QString &a_serial)
{
  if(m_textChangeHook)
    return;

  m_textChangeHook = true;
  m_input->setText (a_serial);
  m_textChangeHook = false;
}

void SerialInput::slotActiavted()
{
  m_input->setHideAnchor (false);
}

/*-----------------------------------------*/
