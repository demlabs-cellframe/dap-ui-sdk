/* INCLUDES */
#include "bugreports.h"
#include "ui_bugreports.h"

#include <QTimer>

/* VARS */
static BugReports *__inst = nullptr;
#define MAX_LENGTH (200)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

const QString BugReports::SCREEN_NAME = "BugReports";

BugReports::BugReports (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::BugReports),
  movLoading (new QMovie (":/gui/ui/asset/Spinner.gif"))
{
  /* setup */
  __inst  = this;
  ui->setupUi (this);
  m_edit  = ui->editReport->editWidget();
  qRegisterMetaType<Mode> ("Mode");
  ui->labelLoading->setMovie (movLoading);
  ui->top_spacer_debug->setVisible (false);
  QTimer::singleShot (0, ui->scrollArea, &BugReportsModel::slotSetup);

  /* fill map */
  m_map.insert (Write,   ui->btnAttachScreenshot);
  m_map.insert (Write,   ui->btnSendReport);
  m_map.insert (Write,   ui->editReport);
  m_map.insert (Write,   ui->labelLetterAmount);
  m_map.insert (List,    ui->scrollArea);
  m_map.insert (Loading, ui->labelLoading);

  /* signals */
  connect (ui->radioTestList, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);
  connect (ui->radioTestWrite, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);
  connect (ui->radioTestLoading, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);

  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &BugReports::sigReturn,
           Qt::QueuedConnection);
  connect (ui->btnSendReport, &KelGuiPushButton::clicked,
           this, &BugReports::sigSend,
           Qt::QueuedConnection);

  connect (m_edit, &QPlainTextEdit::textChanged,
           this, &BugReports::_slotTextChanged);

  /* finish setup */
  slotSetMode (Write);
}

BugReports::~BugReports()
{
  delete ui;
  delete movLoading;
}

/********************************************
 * METHODS
 *******************************************/

BugReports *BugReports::instance()
{
  return __inst;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void BugReports::slotSetMode (BugReports::Mode mode)
{
  /* store */
  m_mode = mode;

  /* prepare hidings */
  QList<Mode> hidden = {List, Write, Loading};
  hidden.removeOne (mode);

  /* show/hide items in cycle */
  for (int ns = 0; ns < 3; ns++)
    {
      /*
       * get values based on hidden or current mode
       * third cycle will be current mode
       */
      auto values =
        m_map.values (
          (ns < 2)
          ? (hidden.at (ns))
          : (mode));

      /* set visisble only on third cycle */
      foreach (auto *w, values)
        w->setVisible (ns == 2);
    }

  /* movie */
  //movLoading->setFileName(":/gui/ui/asset/Spinner.gif");
  //ui->labelLoading->setMovie (movLoading);
  if (m_mode == Loading)
    movLoading->start();
  else
    movLoading->stop();
}

void BugReports::_slotRadioTest()
{
  /* fill whne required */
  if (p_radioTestToMode.empty())
    {
      p_radioTestToMode =
      {
        {ui->radioTestList, List},
        {ui->radioTestWrite, Write},
        {ui->radioTestLoading, Loading},
      };
    }

  /* setup mode */
  slotSetMode (p_radioTestToMode.value (sender(), List));
}

void BugReports::_slotTextChanged()
{
  /* get text */
  auto text = m_edit->toPlainText();

  /* print length */
  ui->labelLetterAmount->setText (
    QString("%1/%2")
    .arg (text.length())
    .arg (MAX_LENGTH));

  /* check if limit reachced */
  if(text.length() <= MAX_LENGTH)
    return;

  /* fix text length */
  int diff  = text.length() - MAX_LENGTH;
  text.chop (diff);
  m_edit->setPlainText (text);

  /* fix cursor pos */
  auto cur  = m_edit->textCursor();
  cur.movePosition (QTextCursor::End, QTextCursor::MoveAnchor);
  m_edit->setTextCursor (cur);
}

/*-----------------------------------------*/
