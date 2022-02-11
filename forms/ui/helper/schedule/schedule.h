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

// view filter
#define FILTER_SIZE 3
class ViewFilter
{
public:
    ViewFilter() { clear(); }
    void push(quint64 data)
    {
        for (int k = 0; k < FILTER_SIZE - 1; k++)
            sample[k] = sample[k + 1];
        sample[FILTER_SIZE - 1] = data;
    }
    quint64 sum()
    {
        quint64 d = 0;
        for (int k = 0; k < FILTER_SIZE; k++)
            d += sample[k];
        return (quint64) d / FILTER_SIZE;
    }
    void clear()
    {
        for (int k = 0; k < FILTER_SIZE; k++)
            sample[k] = 0;
    }
private:
    quint64 sample[FILTER_SIZE];
};

///Adjustments were made, std::list was replaced with List ,and the reverse graph was made to close the line.
/// You need to redo and refactor the code.
///
class Schedule
{
    QList<SheduleElement> m_elems;
    quint64 s_time;
    quint64 s_sample0 = 0;
    quint64 s_sample1 = 0;
    ViewFilter viewfilter;


public:
    Schedule();
    void addElem(quint64 elem);
    int size() {return m_elems.size();}
    void reset();

    int maxValue();
    void showChart(QGraphicsScene *scene, QPen pen, QColor color, int width, int height, int maxVal);
    void updateSample();
};

#endif // SCHEDULE_H
