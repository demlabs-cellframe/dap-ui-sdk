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

#include <cmath>



#define EPSILON 1.0e-5
#define RESOLUTION 8

class ChartPoint2D
{
public:
    double x, y;

    ChartPoint2D() { x = 0.0; y = 0.0; };
    ChartPoint2D(double _x, double _y) { x = _x; y = _y; };

    ChartPoint2D operator +(const ChartPoint2D &point) const { return ChartPoint2D(x + point.x, y + point.y); };
    ChartPoint2D operator -(const ChartPoint2D &point) const { return ChartPoint2D(x - point.x, y - point.y); };
    ChartPoint2D operator *(double v) const { return ChartPoint2D(x * v, y * v); };
    void operator +=(const ChartPoint2D &point) { x += point.x; y += point.y; };
    void operator -=(const ChartPoint2D &point) { x -= point.x; y -= point.y; };

    void normalize()
    {
        double l = sqrt(x * x + y * y);
        x /= l;
        y /= l;
    }
};

class Segment
{
public:
    ChartPoint2D points[4];
    Segment(ChartPoint2D p0, ChartPoint2D p1, ChartPoint2D p2, ChartPoint2D p3)
    {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
        points[3] = p3;
    }
    Segment(const Segment& segment)
    {
        for (int k = 0; k < 4; k++)
            points[k] = segment.points[k];
    }
    Segment &operator=(const Segment& segment)
    {
        for (int k = 0; k < 4; k++)
            points[k] = segment.points[k];
        return *this;
    }
    Segment &operator=(Segment&& segment)
    {
        for (int k = 0; k < 4; k++)
            points[k] = segment.points[k];
        return *this;
    }

    void calc(double t, ChartPoint2D &p)
    {
        double t2 = t * t;
        double t3 = t2 * t;
        double nt = 1.0 - t;
        double nt2 = nt * nt;
        double nt3 = nt2 * nt;
        p.x = nt3 * points[0].x + 3.0 * t * nt2 * points[1].x + 3.0 * t2 * nt * points[2].x + t3 * points[3].x;
        p.y = nt3 * points[0].y + 3.0 * t * nt2 * points[1].y + 3.0 * t2 * nt * points[2].y + t3 * points[3].y;
    };
};

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
    ViewFilter viewfilter;


public:
    Schedule();
    void addElem(quint64 elem);
    int size() {return m_elems.size();}
    void reset();

    int maxValue();
    void showChart(QGraphicsScene *scene, QPen pen, QColor color, int width, int height, int maxVal);
};

#endif // SCHEDULE_H
