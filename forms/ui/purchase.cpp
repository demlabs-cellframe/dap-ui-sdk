/* INCLUDES */
#include "purchase.h"
#include "ui_purchase.h"

#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Purchase::Purchase (QWidget *parent) :
  BaseForm (parent),
  ui (new Ui::Purchase)
{
  ui->setupUi (this);
  QTimer::singleShot (0, ui->scrollArea, &PurchaseModel::slotSetup);

  /* signals */
  connect (ui->btnReturn, &DapGuiPushButton::clicked,
           this, &Purchase::sigReturn,
           Qt::QueuedConnection);
}

Purchase::~Purchase()
{
  delete ui;
}

/*-----------------------------------------*/
