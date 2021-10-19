/* INCLUDES */
#include "history.h"
#include "ui_history.h"

#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

History::History (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::History)
{
  ui->setupUi (this);
  QTimer::singleShot (0, ui->scrollArea, &HistoryModel::slotSetup);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &History::sigReturn,
           Qt::QueuedConnection);
}

History::~History()
{
  delete ui;
}

/*-----------------------------------------*/
