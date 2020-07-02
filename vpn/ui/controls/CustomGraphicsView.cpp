#include "CustomGraphicsView.h"

CustomGraphicsView::CustomGraphicsView(QWidget *a_parent)
    :QGraphicsView(a_parent)
{

}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    this->setSceneRect(0,0,this->width(),this->height());
    QGraphicsView::resizeEvent(event);
}
