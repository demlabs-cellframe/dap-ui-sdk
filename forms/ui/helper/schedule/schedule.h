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

#define SAMPLE_LEN 40

#define EPSILON 1.0e-5
#define RESOLUTION 8

class ChartPoint2D
{
public:
    qreal x, y;

    ChartPoint2D() { x = 0.0; y = 0.0; };
    ChartPoint2D(qreal _x, qreal _y) { x = _x; y = _y; };

    ChartPoint2D operator +(const ChartPoint2D &point) const { return ChartPoint2D(x + point.x, y + point.y); };
    ChartPoint2D operator -(const ChartPoint2D &point) const { return ChartPoint2D(x - point.x, y - point.y); };
    ChartPoint2D operator *(double v) const { return ChartPoint2D(x * v, y * v); };
    void operator +=(const ChartPoint2D &point) { x += point.x; y += point.y; };
    void operator -=(const ChartPoint2D &point) { x -= point.x; y -= point.y; };

    void normalize()
    {
        qreal l = sqrt(x * x + y * y);
        if (l == 0)
        {
            x = 0;
            y = 0;
        }
        else
        {
            x /= l;
            y /= l;
        }
    }
};

class FourPointsSegment
{
public:
    ChartPoint2D points[4];
    FourPointsSegment(ChartPoint2D p0, ChartPoint2D p1, ChartPoint2D p2, ChartPoint2D p3)
    {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
        points[3] = p3;
    }
    FourPointsSegment(const FourPointsSegment& segment)
    {
        for (int k = 0; k < 4; k++)
            points[k] = segment.points[k];
    }
    FourPointsSegment &operator=(const FourPointsSegment& segment)
    {
        for (int k = 0; k < 4; k++)
            points[k] = segment.points[k];
        return *this;
    }
    FourPointsSegment &operator=(FourPointsSegment&& segment)
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


///Adjustments were made, std::list was replaced with List ,and the reverse graph was made to close the line.
/// You need to redo and refactor the code.
///
class Schedule
{
    QList<SheduleElement> m_elems;
    QList<SheduleElement> m_interpolationPoints;
    quint64 m_graphStartTime;

public:
    Schedule();
    void addElem(quint64 elem);
    int size() {return m_elems.size();}
    void reset();
    qreal maxValue();
    void showChart(QGraphicsScene *scene, QPen pen, QColor color, int width, int height, qreal maxVal);
};

#endif // SCHEDULE_H
