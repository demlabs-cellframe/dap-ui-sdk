#include <QtMath>
#include "schedule.h"
#include "DapSpeed.h"


bool calculateSpline(const QList<ChartPoint2D> &values, QList<FourPointsSegment> &bezier)
{
    int valuesSize = values.size() - 1;

    if (valuesSize < 2)
        return false;

    ChartPoint2D tgL;
    ChartPoint2D tgR;
    ChartPoint2D cur;
    ChartPoint2D next = values[1] - values[0];
    next.normalize();

    --valuesSize;

    qreal l1, l2, tmp, x;

    for (int i = 0; i < valuesSize; ++i)
    {
        bezier.push_back(FourPointsSegment(values[i], values[i],
                                 values[i + 1], values[i + 1]));

        cur = next;
        next = values[i + 2] - values[i + 1];
        next.normalize();

        tgL = tgR;

        tgR = cur + next;
        tgR.normalize();

        if (qFabs(values[i + 1].y - values[i].y) < EPSILON)
        {
            l1 = l2 = 0.0;
        }
        else
        {
            tmp = values[i + 1].x - values[i].x;
            l1 = qFabs(tgL.x) > EPSILON ? tmp / (2.0 * tgL.x) : 1.0;
            l2 = qFabs(tgR.x) > EPSILON ? tmp / (2.0 * tgR.x) : 1.0;
        }

        if (qFabs(tgL.x) > EPSILON && qFabs(tgR.x) > EPSILON)
        {
            tmp = tgL.y / tgL.x - tgR.y / tgR.x;
            if (qFabs(tmp) > EPSILON)
            {
                x = (values[i + 1].y - tgR.y / tgR.x * values[i + 1].x - values[i].y + tgL.y / tgL.x * values[i].x) / tmp;
                if (x > values[i].x && x < values[i + 1].x)
                {
                    if (tgL.y > 0.0)
                    {
                        if (l1 > l2)
                            l1 = 0.0;
                        else
                            l2 = 0.0;
                    }
                    else
                    {
                        if (l1 < l2)
                            l1 = 0.0;
                        else
                            l2 = 0.0;
                    }
                }
            }
        }

        bezier[i].points[1] += tgL * l1;
        bezier[i].points[2] -= tgR * l2;
    }

    l1 = qFabs(tgL.x) > EPSILON ? (values[valuesSize + 1].x - values[valuesSize].x) / (2.0 * tgL.x) : 1.0;

    bezier.push_back(FourPointsSegment(values[valuesSize], values[valuesSize],
                             values[valuesSize + 1], values[valuesSize + 1]));
    bezier[valuesSize].points[1] += tgR * l1;

    return true;
}


void Interpolation(QList<SheduleElement> &chartData, QList<SheduleElement> &result)
{
    QList<ChartPoint2D> charPoints;
    QList<FourPointsSegment> spline;
    ChartPoint2D p;
    for (int k = 0; k < chartData.size(); k++)
        charPoints.push_front(ChartPoint2D(chartData[k].time, chartData[k].velocity));

    calculateSpline(charPoints, spline);

    foreach (FourPointsSegment s, spline)
    {
        for (int i = 0; i < RESOLUTION; ++i)
        {
            s.calc((double)i / (double)RESOLUTION, p);
            result.push_front(SheduleElement(p.x, p.y));
        }
    }
}


Schedule::Schedule():
    m_graphStartTime(0)
{
    m_interpolationPoints.reserve(SAMPLE_LEN * RESOLUTION);
    m_elems.push_front(SheduleElement (0, 0));
}

void Schedule::addElem(quint64 speed)
{
    quint64 newTime = CurrentMillisecond();
    if (m_graphStartTime == 0)
        m_graphStartTime = newTime;
    m_elems.push_front(SheduleElement(newTime - m_graphStartTime, (qreal)speed));
    if (m_elems.size() > SAMPLE_LEN)
        m_elems.pop_back();
}

void Schedule::reset()
{
    m_elems.clear();
    m_elems.push_front(SheduleElement (0, 0));
    m_graphStartTime = 0;
}

qreal Schedule::maxValue()
{
    if (size() == 0) {
        return 0;
    }

    qreal maxVal = m_elems.begin()->velocity;
    QList<SheduleElement>::iterator ptr;

    for (ptr = m_elems.begin(); ptr != m_elems.end(); ptr++) {
        if (ptr->velocity > maxVal) maxVal = ptr->velocity;
    }
    if (maxVal < 10) maxVal = 10;
    return maxVal;
}

qreal x_shift(qreal x, qreal width, qreal number_of_elem)
{
    if (number_of_elem == 0) {
        number_of_elem = 1;
    }

    qreal res = x * width / number_of_elem;
    return res;
}

qreal y_shift(qreal y, qreal height, qreal maxValue)
{
    if (maxValue == 0) {
        maxValue = 1;
    }
    qreal res = height - y * height / maxValue;
    return res;
}


void Schedule::showChart(
        QGraphicsScene *scene, QPen pen, QColor color, int width, int height, qreal maxVal)
{
    m_interpolationPoints.clear();
    Interpolation(m_elems, m_interpolationPoints);
    if (m_interpolationPoints.size() < 2)
        return;
    int size_of_chart = m_interpolationPoints.size();
    QPainterPath path = QPainterPath();
    path.moveTo(
                width,
                y_shift(m_interpolationPoints.begin()->velocity, height, maxVal)
                );
    qreal time_pos = size_of_chart - 2;
    for(int i = 1;i<m_interpolationPoints.size();i++)
    {
        qreal y = 0;
        if (m_interpolationPoints[i].velocity > y) y = m_interpolationPoints[i].velocity;

        path.lineTo(
                    x_shift(time_pos, width, size_of_chart),
                    y_shift(y, height, maxVal));
        time_pos--;
    }
    time_pos = 1;
    for(int i = m_interpolationPoints.size()-2;i>0;i--)
    {
        qreal y = m_interpolationPoints[i].velocity;
        path.lineTo(
                    x_shift(time_pos, width, size_of_chart),
                    y_shift(y, height, maxVal));
        time_pos++;
    }
    path.closeSubpath();
    QPen a_pen = QPen(pen);
    a_pen.setJoinStyle(Qt::RoundJoin);
#ifdef Q_OS_ANDROID
    a_pen.setWidth(5);
#else
    a_pen.setWidth(2);
#endif
    scene->addPath(path, a_pen, color);
}

