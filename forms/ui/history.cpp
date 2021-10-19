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
  QMetaObject::invokeMethod(ui->scrollArea, &HistoryModel::slotSetup, Qt::QueuedConnection);

  /* signals */
  connect (ui->btnReturn, &KelGuiPushButton::clicked,
           this, &History::sigReturn,
           Qt::QueuedConnection);
}

History::~History()
{
  delete ui;
}

/*-----------------------------------------*/
