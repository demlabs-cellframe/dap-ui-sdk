#include "ScreenOverlaying.h"

ScreenOverlaying::ScreenOverlaying(QWidget *parent):QWidget(parent) {
}

void ScreenOverlaying::mousePressEvent(QMouseEvent *event){
    Q_UNUSED(event);
    emit mousePressed();
}

void ScreenOverlaying::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.fillRect(event->rect(), palette().color(QPalette::Window));
}
