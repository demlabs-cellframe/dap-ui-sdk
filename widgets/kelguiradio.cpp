/* INCLUDES */
#include "kelguiradio.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiRadio::KelGuiRadio (QWidget *parent) :
  QRadioButton (parent)
{
  __kgsm.forcedRadioStyleUpdate();
}

/*-----------------------------------------*/
