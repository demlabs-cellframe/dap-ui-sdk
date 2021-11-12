/* INCLUDES */
#include "dapguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiGraphicsView::DapGuiGraphicsView (QWidget *parent) :
  QGraphicsView (parent)
{
  __kgsm.forcedGraphicsViewStyleUpdate();
}

/********************************************
 * OVERRIDE
 *******************************************/

void DapGuiGraphicsView::resizeEvent (QResizeEvent *event)
{
  QGraphicsView::resizeEvent (event);
  __kgsm.forcedGraphicsViewStyleUpdate();
}

/*-----------------------------------------*/
