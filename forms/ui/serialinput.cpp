/* INCLUDES */
#include "serialinput.h"
#include "ui_serialinput.h"
#include "kelguilineedit.h"
#include <QDebug>
#include <QKeyEvent>
#include <QClipboard>

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
  ui->MainWidget->layout()->removeWidget (ui->customInputBorder);
  ui->btnSerial->setEdit (ui->customInputBorder);

  /* store input ptr */
  m_input = ui->customInput;
  m_input->setText ("");

  /* signals */

  connect (this, &SerialInput::sigReturn,
           this, &SerialInput::slotCloseInput,
           Qt::QueuedConnection);

  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &SerialInput::sigReturn,
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

void SerialInput::slotSetSerial(const QString &a_serial)
{
  if(m_textChangeHook)
    return;

  m_textChangeHook = true;
  qDebug() << __PRETTY_FUNCTION__ << "start" << a_serial;

  m_input->setText (a_serial);

  qDebug() << __PRETTY_FUNCTION__ << "finish";
  m_textChangeHook = false;
}

void SerialInput::slotCloseInput()
{
  QGuiApplication::inputMethod()->hide();
}

/*-----------------------------------------*/
