/* INCLUDES */
#include "kelguiseparator.h"
#include "ui_kelguiseparator.h"
#include <QStyleOption>
#include <QPainter>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiSeparator::KelGuiSeparator (QWidget *parent)
  : QWidget (parent)
  , ui (new Ui::KelGuiSeparatorUI)
{
  ui->setupUi (this);
  setMaximumHeight (4);
//  auto sz     = QPixmap ("://gfx/separator.png").size();
//  auto style  = QString ("background-image: url(://gfx/separator.png);"
//                         "min-width: %1px;min-height: %2px;"
//                         "max-width: %1px;max-height: %2px;")
//                .arg (sz.width())
//                .arg (sz.height());
//  setStyleSheet(style);
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiSeparator::paintEvent (QPaintEvent *)
{
  QStyleOption opt;
  opt.init (this);

  QPainter p (this);
  style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

/*-----------------------------------------*/
