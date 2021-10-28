/* INCLUDES */
#include "bugreports.h"
#include "ui_bugreports.h"

#include <QTimer>
#include <QDebug>
#include <QScrollBar>

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
  movLoading (new QMovie (":/gui/ui/asset/Spinner.gif")),
  _textHook (false)
{
  qRegisterMetaType<Mode> ("Mode");

  /* setup */
  __inst  = this;
  ui->setupUi (this);

  ui->editReport->setPlainText ("");
  ui->editReport->setCallbackTextEdit (_cbTextEdit);
  m_edit  = ui->editReport->editWidget();
  //m_edit->verticalScrollBar()->setMaximumWidth (width() / 10);
  m_edit->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
  m_edit->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

  ui->labelLoading->setScaledContents (true);
  ui->labelLoading->setMovie (movLoading);
  //movLoading->start();

  ui->top_spacer_debug->setVisible (false);
  ui->btnAttachScreenshot->setVisible (false);
  QMetaObject::invokeMethod(ui->scrollArea, &BugReportsModel::slotSetup, Qt::QueuedConnection);
  //setText (movLoading->lastErrorString());

  /* fill map */
  //m_map.insert (Write,   ui->btnAttachScreenshot);
  m_map.insert (Write,   ui->btnSendReport);
  m_map.insert (Write,   ui->editReport);
  m_map.insert (Write,   ui->labelLetterAmount);
  m_map.insert (List,    ui->scrollArea);
  m_map.insert (Loading, ui->labelLoading);
  m_map.insert (Result,  ui->lResult);
  m_map.insert (Result,  ui->btnResultBack);

  /* signals */
  connect (ui->radioTestList, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);
  connect (ui->radioTestWrite, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);
  connect (ui->radioTestLoading, &QRadioButton::clicked,
           this, &BugReports::_slotRadioTest);

  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &BugReports::sigReturn,
           Qt::QueuedConnection);
  connect (ui->btnSendReport, &DapGuiPushButton::clicked,
           this, &BugReports::sigSend,
           Qt::QueuedConnection);
  connect (ui->btnResultBack, &DapGuiPushButton::clicked,
           this, &BugReports::sigResultBack,
           Qt::QueuedConnection);

  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &BugReports::_slotTextEditFinish,
           Qt::QueuedConnection);
  connect (ui->btnSendReport, &DapGuiPushButton::clicked,
           this, &BugReports::_slotTextEditFinish,
           Qt::QueuedConnection);
  connect (ui->btnResultBack, &DapGuiPushButton::clicked,
           this, &BugReports::_slotTextEditFinish,
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

QString BugReports::text() const
{
  return ui->editReport->plainText();
}

void BugReports::setText(const QString &a_text)
{
  ui->editReport->setPlainText (a_text);
}

void BugReports::setResultText(const QString &a_result)
{
  ui->lResult->setText(a_result);
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void BugReports::slotSetMode (BugReports::Mode mode)
{
  /* store */
  m_mode = mode;

  /* prepare hidings */
  QList<Mode> hidden = {List, Write, Loading, Result};
  hidden.removeOne (mode);

  /* show/hide items in cycle */
  for (int ns = 0; ns < 4; ns++)
    {
      /*
       * get values based on hidden or current mode
       * third cycle will be current mode
       */
      auto values =
        m_map.values (
          (ns < 3)
          ? (hidden.at (ns))
          : (mode));

      /* set visisble only on third cycle */
      foreach (auto *w, values)
        w->setVisible (ns == 3);
    }

  /* movie */
  int w   = ui->labelLoading->width();
  movLoading->setFileName(":/gui/ui/asset/Spinner.gif");
  movLoading->setScaledSize (QSize (w, w));

  qDebug() << __PRETTY_FUNCTION__ << "spinner width:" << w;

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
  auto text = m_edit->toPlainText();
  updateData (text, text.length());
}

void BugReports::_slotTextEditFinish()
{
  ui->editReport->unfocus();
}

bool BugReports::_cbTextEdit (
    DapGuiPlainTextEditInterface *e,
    QString &preedit,
    QString &commit,
    int from,
    int to)
{
  Q_UNUSED(from)
  Q_UNUSED(to)

  /* get length only */
  auto text = e->toPlainText() + preedit + commit;
  auto len  = text.length();

  /* update only by length */
  text.clear();
  __inst->updateData (text, len);
  return false;
}

void BugReports::updateData (QString &a_text, int a_len)
{
  if(_textHook)
    return;

  /* print length */
  auto len  = (a_len < MAX_LENGTH) ? a_len : MAX_LENGTH;
  ui->labelLetterAmount->setText (
    QString("%1/%2")
    .arg (len)
    .arg (MAX_LENGTH));
  //ui->labelLetterAmount->repaint();
  //style()->polish (ui->labelLetterAmount);

  /* check if limit reachced */
  if(a_text.length() <= MAX_LENGTH)
    return;

  _textHook = true;

  /* fix text length */
  int diff  = a_text.length() - MAX_LENGTH;
  a_text.chop (diff);
  m_edit->setPlainText (a_text);

  /* kill focus */
  ui->editReport->unfocus();
  QGuiApplication::inputMethod()->hide();

  /* fix cursor pos */
  QMetaObject::invokeMethod(this, [=] () {
      auto cur = m_edit->textCursor();
      cur.movePosition (QTextCursor::End, QTextCursor::MoveAnchor);
      m_edit->setTextCursor (cur);
  }, Qt::QueuedConnection);

  _textHook = false;
}

/*-----------------------------------------*/
