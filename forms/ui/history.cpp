/* INCLUDES */
#include "history.h"
#include "ui_history.h"

#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

History::History(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::History)
{
  ui->setupUi(this);
  QTimer::singleShot (0, ui->scrollArea, &HistoryModel::slotSetup);
}

History::~History()
{
  delete ui;
}

/*-----------------------------------------*/
