/* INCLUDES */
#include "kelguicheckbox.h"

/* FUNCS */
QString fromFile (const QString &filename);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiCheckBox::KelGuiCheckBox(QWidget *parent) :
  QCheckBox(parent)
{
  auto sz     = QPixmap ("://gfx/check_btn_on.png").size();
  auto style  = QString (fromFile ("://styles/checkbox.css"))
                .arg (sz.width())
                .arg (sz.height());
  setStyleSheet(style);
}

/*-----------------------------------------*/
