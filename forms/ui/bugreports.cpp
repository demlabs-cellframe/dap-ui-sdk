/* INCLUDES */
#include "bugreports.h"
#include "ui_bugreports.h"

#include <QTimer>
#include <QDebug>
#include <QScrollBar>
#include <QScroller>

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
  movLoading (new QMovie (":/gui/asset/Spinner.gif")),
  _textHook (false),
  _spacer (true)
{
  /* setup */
  __inst  = this;
  ui->setupUi (this);

  qRegisterMetaType<Mode> ("Mode");
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

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

  ui->btnSendReport->setEnabled(false);

  /* fill map */
  //m_map.insert (Write,   ui->btnAttachScreenshot);
  m_map.insert (Write,   ui->btnSendReport);
  m_map.insert (Write,   ui->editReport);
  m_map.insert (Write,   ui->labelLetterAmount);
  m_map.insert (List,    ui->scrollArea);
  m_map.insert (Loading, ui->labelLoading);
  m_map.insert (Loading, ui->btnCancel);
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
  connect (ui->btnCancel, &DapGuiPushButton::clicked,
           this, &BugReports::sigResultBack,
           Qt::QueuedConnection);
  connect (ui->btnResultBack, &DapGuiPushButton::clicked,
           this, &BugReports::sigReturn,
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

  /* spacer */
  if (mode == List)
    {
      if (_spacer)
        {
          ui->verticalLayout_3->takeAt (ui->verticalLayout_3->count()-1);
          _spacer = false;
        }
    }
  else
    {
      if (!_spacer)
        {
          ui->verticalLayout_3->addItem (ui->verticalSpacer);
          _spacer = true;
        }
    }

  /* movie */
  int w   = ui->labelLoading->width();
  movLoading->setFileName(":/gui/asset/Spinner.gif");
  movLoading->setScaledSize (QSize (w, w));

  qDebug() << __PRETTY_FUNCTION__ << "spinner width:" << w;

  if (m_mode == Loading){
    ui->lResult->setVisible(true);
    ui->lResult->setText("Sending...");
    movLoading->start();
  }
  else
    movLoading->stop();
}

void BugReports::slotRetranslated()
{
  ui->labelBugReports->setText (tr ("Bug reports"));
  ui->editReport
      ->setPlaceholderText (
        tr ("Please describe the details of problem you faced. "
            "What actions did you take and what happened."));
  ui->btnAttachScreenshot->setText (tr ("Click here to attach a screenshot"));
  ui->btnSendReport->setText (tr ("send report"));
  ui->btnResultBack->setText (tr ("back"));
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

  ui->btnSendReport->setEnabled(!a_text.isEmpty());

  /* check if limit reachced */
  if(a_text.length() <= MAX_LENGTH)
  {
    m_bugReportText = a_text;
    return;
  }

  _textHook = true;

  /* fix text length */
  int position = m_edit->textCursor().position();
  m_edit->setPlainText(m_bugReportText);
  QTextCursor cur = m_edit->textCursor();
  cur.setPosition(position-1);
  m_edit->setTextCursor(cur);

  _textHook = false;
}

void BugReports::refreshHistoryList(){
    QMetaObject::invokeMethod(ui->scrollArea, &BugReportsModel::slotSetup, Qt::QueuedConnection);
}

/*-----------------------------------------*/
