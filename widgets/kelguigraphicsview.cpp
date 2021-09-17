/* INCLUDES */
#include "kelguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

KelGuiGraphicsView::KelGuiGraphicsView (QWidget *parent) :
  QGraphicsView (parent)
{
  __kgsm.forcedGraphicsViewStyleUpdate();
}

/********************************************
 * OVERRIDE
 *******************************************/

void KelGuiGraphicsView::resizeEvent (QResizeEvent *event)
{
  QGraphicsView::resizeEvent (event);
  __kgsm.forcedGraphicsViewStyleUpdate();
}

/*-----------------------------------------*/
