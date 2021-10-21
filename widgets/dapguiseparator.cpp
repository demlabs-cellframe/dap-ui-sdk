/* INCLUDES */
#include "dapguiseparator.h"
#include "ui_dapguiseparator.h"
#include <QStyleOption>
#include <QPainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiSeparator::DapGuiSeparator (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::DapGuiSeparatorUI)
{
  ui->setupUi (this);
  setMaximumHeight (4);
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiSeparator::paintEvent (QPaintEvent *)
{
  QStyleOption opt;
  opt.init (this);

  QPainter p (this);
  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

/*-----------------------------------------*/
