/* INCLUDES */
#include "bugreports.h"
#include "ui_bugreports.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BugReports::BugReports (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::BugReports),
  movLoading (new QMovie (":/gui/ui/asset/Spinner.gif"))
{
  /* setup */
  ui->setupUi (this);
  qRegisterMetaType<Mode> ("Mode");
  ui->labelLoading->setMovie (movLoading);

  /* fill map */
  m_map.insert (Write,   ui->btnAttachScreenshot);
  m_map.insert (Write,   ui->btnSendReport);
  m_map.insert (Write,   ui->editReport);
  m_map.insert (Write,   ui->labelLetterAmount);
  m_map.insert (List,    ui->listView);
  m_map.insert (Loading, ui->labelLoading);

  /* signals */
  connect (ui->radioTestList, &QRadioButton::clicked,
           this, &BugReports::slotRadioTest);
  connect (ui->radioTestWrite, &QRadioButton::clicked,
           this, &BugReports::slotRadioTest);
  connect (ui->radioTestLoading, &QRadioButton::clicked,
           this, &BugReports::slotRadioTest);

  /* finish setup */
  slotSetMode (Write);
}

BugReports::~BugReports()
{
  delete ui;
  delete movLoading;
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

void BugReports::slotRadioTest()
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

/********************************************
 * OVERRIDE
 *******************************************/

void BugReports::restartUi()
{
  BASEFORM_RESTARTUI_ROUTINE (Ui::BugReports)
//  QLayoutItem *wItem;
//  while ((wItem = layout()->takeAt(0)) != 0)
//  {
//    if(wItem->widget())
//      wItem->widget()->setParent(nullptr);
//    delete wItem;
//  }
}

/*-----------------------------------------*/
