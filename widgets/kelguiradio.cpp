/* INCLUDES */
#include "kelguiradio.h"

/* FUNCS */
QString fromFile (const QString &filename);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiRadio::KelGuiRadio (QWidget *parent) :
  QRadioButton (parent)
{
  auto sz     = QPixmap ("://gfx/radio_btn_on.png").size();
  auto style  = QString (fromFile ("://styles/radiobutton.css"))
                .arg (sz.width())
                .arg (sz.height());
  setStyleSheet(style);
}

/*-----------------------------------------*/
