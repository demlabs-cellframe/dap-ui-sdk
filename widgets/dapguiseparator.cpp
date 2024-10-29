/* INCLUDES */
#include "dapguiseparator.h"
#include "ui_dapguiseparator.h"
#include <ui/helper/auxiliary/UiScaling.h>
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
  auto size = UiScaling::pointsToPixels (4, UiScaling::getNativDPI());
  setMaximumHeight ((size >= 4) ? size : 4);
}

/********************************************
 * OVERRIDE
 *******************************************/

//void DapGuiSeparator::paintEvent (QPaintEvent *)
//{
//  QStyleOption opt;
//  opt.init (this);

//  QPainter p (this);
//  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
//}

/*-----------------------------------------*/
