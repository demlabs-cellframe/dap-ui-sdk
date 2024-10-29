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
  qRegisterMetaType<Product> ("Product");
  ui->setupUi (this);
  QMetaObject::invokeMethod (
    ui->scrollArea,
    "slotSetup",
    Qt::QueuedConnection,
    Q_ARG (QWidget*,this));

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
