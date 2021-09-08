/* INCLUDES */
#include "kelguiradio.h"
#include "kelguicommon.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiRadio::KelGuiRadio (QWidget *parent) :
  QRadioButton (parent)
{
  auto sz     = QPixmap ("://gfx/radio_btn_on.png").size();
  auto style  = QString (Common::fromFile ("://styles/radiobutton.css"))
                .arg (sz.width())
                .arg (sz.height());
  setStyleSheet (style);
}

/*-----------------------------------------*/
