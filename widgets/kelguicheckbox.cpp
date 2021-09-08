/* INCLUDES */
#include "kelguicheckbox.h"
#include "kelguicommon.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiCheckBox::KelGuiCheckBox (QWidget *parent) :
  QCheckBox (parent)
{
  auto sz     = QPixmap ("://gfx/check_btn_on.png").size();
  auto style  = QString (Common::fromFile ("://styles/checkbox.css"))
                .arg (sz.width())
                .arg (sz.height());
  setStyleSheet (style);
}

/*-----------------------------------------*/
