#ifndef SCREENOVERLAYING_H
#define SCREENOVERLAYING_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

//claass wich fill screen under menu
class ScreenOverlaying: public QWidget
{
    Q_OBJECT
public:
    ScreenOverlaying(QWidget *parent = nullptr);

protected:

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

signals:
    void mousePressed();
};


#endif // SCREENOVERLAYING_H
