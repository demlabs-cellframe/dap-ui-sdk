/* INCLUDES */
#include "history.h"
#include "ui_history.h"

#include <QTimer>
#include <QScroller>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

History::History (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::History)
{
  ui->setupUi (this);
  QMetaObject::invokeMethod(ui->scrollArea, &HistoryModel::slotSetup, Qt::QueuedConnection);
  QScroller::grabGesture(this->ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &History::sigReturn,
           Qt::QueuedConnection);

  connect (ui->scrollArea, &HistoryModel::filled, [=](int rowCount)
  {
    ui->scrollArea->setVisible(rowCount > 0);
    ui->noKeyLabel->setVisible(rowCount == 0);
  });
}

History::~History()
{
  delete ui;
}

void History::refreshHistoryList()
{
    QMetaObject::invokeMethod(ui->scrollArea, &HistoryModel::slotSetup, Qt::QueuedConnection);
}

/*-----------------------------------------*/
