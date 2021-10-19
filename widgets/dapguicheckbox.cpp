/* INCLUDES */
#include "dapguicheckbox.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiCheckBox::DapGuiCheckBox (QWidget *parent) :
  QCheckBox (parent)
{
  __kgsm.forcedCheckboxStyleUpdate();
}

/*-----------------------------------------*/
