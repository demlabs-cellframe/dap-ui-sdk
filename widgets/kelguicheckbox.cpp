/* INCLUDES */
#include "kelguicheckbox.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiCheckBox::KelGuiCheckBox (QWidget *parent) :
  QCheckBox (parent)
{
  __kgsm.forcedCheckboxStyleUpdate();
}

/*-----------------------------------------*/
