/* INCLUDES */
#include "modelbase.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ModelBase::ModelBase (QWidget *parent)
  : QScrollArea (parent),
    lay (nullptr)
{

}

/********************************************
 * PROTECTED METHODS
 *******************************************/

void ModelBase::setupLayout()
{
  /* setup model */
  if (lay)
    return;

  /* if widget exists */
  if (widget())
    {
      /* if layout not present */
      if (!layout())
        {
          /* cerate and store */
          lay    = new QVBoxLayout (widget());
          lay->setMargin (0);
          lay->setSpacing (0);
          widget()->setLayout (lay);
          widget()->installEventFilter (this);
        }

      /* get existing layout */
      else
        lay = dynamic_cast<QVBoxLayout *> (widget()->layout());
    }
  /* no widget -> no model job */
  else
    return;
}

void ModelBase::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
        delete item;
}

/*-----------------------------------------*/
