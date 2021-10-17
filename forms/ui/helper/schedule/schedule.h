#ifndef SCHEDULE_H
#define SCHEDULE_H


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsPathItem>
#ifdef Q_OS_WIN
#include <ctime>
#endif
#include "SheduleElement.h"

///Adjustments were made, std::list was replaced with List ,and the reverse graph was made to close the line.
/// You need to redo and refactor the code.
///
// Класс для хранения и отрисовки меток скорости на графике.
class Schedule
{
    QList<SheduleElement> m_elems;
    time_t s_time     = time(nullptr);
    quint64 s_quantity = 0;
    int diff       = 0;


public:
    Schedule();
    void addElem(quint64 elem);
    int size() {return m_elems.size();}
    void reset();

    int maxValue();
    void showChart(QGraphicsScene *scene, QPen pen, QColor color, int width, int height, int maxVal);
};

#endif // SCHEDULE_H
