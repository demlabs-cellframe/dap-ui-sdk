/**************************************************************************************
** This class was created for automatically setting the scene size.
****************************************************************************************/

#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QDebug>

class CustomGraphicsView: public QGraphicsView
{
    Q_OBJECT

public:
    CustomGraphicsView(QWidget *a_parent);

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // CUSTOMGRAPHICSVIEW_H
