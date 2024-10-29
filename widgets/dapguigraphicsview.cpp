/* INCLUDES */
#include "dapguigraphicsview.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiGraphicsView::DapGuiGraphicsView (QWidget *parent)
  : QGraphicsView (parent)
  , m_form (nullptr)
{
  __kgsm.forcedGraphicsViewStyleUpdate();
}

/********************************************
 * METHODS
 *******************************************/

QObject *DapGuiGraphicsView::form() const
{
  return m_form;
}

void DapGuiGraphicsView::setForm(QObject *a_form)
{
  m_form  = a_form;
}

void DapGuiGraphicsView::updateG()
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
