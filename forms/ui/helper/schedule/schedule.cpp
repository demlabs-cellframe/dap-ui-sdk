#include "schedule.h"
#include "DapSpeed.h"


bool calculateSpline(const QList<ChartPoint2D> &values, QList<Segment> &bezier)
{
    int valuesSize = values.size() - 1;

    if (valuesSize < 2)
        return false;

//    bezier.resize(n);

    ChartPoint2D tgL;
    ChartPoint2D tgR;
    ChartPoint2D cur;
    ChartPoint2D next = values[1] - values[0];
    next.normalize();

    --valuesSize;

    double l1, l2, tmp, x;

    for (int i = 0; i < valuesSize; ++i)
    {
        bezier.push_back(Segment(values[i], values[i],
                                 values[i + 1], values[i + 1]));

        cur = next;
        next = values[i + 2] - values[i + 1];
        next.normalize();

        tgL = tgR;

        tgR = cur + next;
        tgR.normalize();

        if (abs(values[i + 1].y - values[i].y) < EPSILON)
        {
            l1 = l2 = 0.0;
        }
        else
        {
            tmp = values[i + 1].x - values[i].x;
            l1 = abs(tgL.x) > EPSILON ? tmp / (2.0 * tgL.x) : 1.0;
            l2 = abs(tgR.x) > EPSILON ? tmp / (2.0 * tgR.x) : 1.0;
        }

        if (abs(tgL.x) > EPSILON && abs(tgR.x) > EPSILON)
        {
            tmp = tgL.y / tgL.x - tgR.y / tgR.x;
            if (abs(tmp) > EPSILON)
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

    l1 = abs(tgL.x) > EPSILON ? (values[valuesSize + 1].x - values[valuesSize].x) / (2.0 * tgL.x) : 1.0;

    bezier.push_back(Segment(values[valuesSize], values[valuesSize],
                             values[valuesSize + 1], values[valuesSize + 1]));
    bezier[valuesSize].points[1] += tgR * l1;

    return true;
}


void Interpolation(QList<SheduleElement> &chartData, QList<SheduleElement> &result)
{
    QList<ChartPoint2D> charPoints;
    QList<Segment> spline;
    ChartPoint2D p;
    time_t a = chartData.back().time;
    for (int k = 0; k < chartData.size(); k++)
    {
        charPoints.push_front(ChartPoint2D(chartData[k].time - a, chartData[k].velocity));
    }
    calculateSpline(charPoints, spline);
    qDebug() << "calculateSpline";
    for (auto s : spline)
    {
        for (int i = 0; i < RESOLUTION; ++i)
        {
            s.calc((double)i / (double)RESOLUTION, p);
            result.push_front(SheduleElement((quint64) abs(p.x * 10), abs(p.y * 10)));
        }
    }
    for (int k = 0; k < result.size(); k++)
        qDebug() << k << result[k].velocity << result[k].time;
}


Schedule::Schedule()
{
  m_elems.push_front(SheduleElement (time (nullptr), 0));
}

void Schedule::addElem(quint64 newQuantity)
{
//    viewfilter.push(newQuantity);
    quint64 newTime = CurrentMillisecond();
//    m_elems.push_front(SheduleElement(newTime, viewfilter.sum()));
    m_elems.push_front(SheduleElement(newTime, newQuantity));
    if (m_elems.size() > 40)
        m_elems.pop_back();
}

void Schedule::reset()
{
  m_elems.clear();
  m_elems.push_front(SheduleElement (time (nullptr), 0));
  viewfilter.clear();
}

int Schedule::maxValue()
{
    if (size() == 0) {
        return 0;
    }

    int maxVal = m_elems.begin()->velocity;
    QList<SheduleElement>::iterator ptr;

    for (ptr = m_elems.begin(); ptr != m_elems.end(); ptr++) {
        if (ptr->velocity > maxVal) maxVal = ptr->velocity;
    }
    return maxVal;
}

qreal x_shift(int x, qreal width, qreal number_of_elem)
{
    if (number_of_elem == 0) {
        number_of_elem = 1;
    }

    int res = x * width / number_of_elem;
    return res;
}

qreal y_shift(qreal y, qreal height, qreal maxValue)
{
    if (maxValue == 0) {
        maxValue = 1;
    }
    int res = height - y * height / maxValue;
    return res;
}


void Schedule::showChart(
        QGraphicsScene *scene, QPen pen, QColor color, int width, int height, int maxVal)
{
    QList<SheduleElement> itrp;
    Interpolation(m_elems, itrp);
    if (itrp.size() < 5)
        return;
    int size_of_chart = itrp.size();//size();
    QPainterPath path = QPainterPath();
    qDebug() << "QPainterPath path = QPainterPath();" <<                width <<
            y_shift(itrp.begin()->velocity, height, maxVal);
    path.moveTo(
                width,
                y_shift(itrp.begin()->velocity, height, maxVal)
                );
    int time_pos = size_of_chart - 2;
    qDebug() << "int time_pos = size_of_chart - 2;    ";
    for(int i = 1;i<itrp.size();i++)
    {
        int y = 0;
        if (itrp[i].velocity > y) y = itrp[i].velocity;

        path.lineTo(
                    x_shift(time_pos, width, size_of_chart),
                    y_shift(y, height, maxVal));
        time_pos--;
    }
    time_pos = 1;
    for(int i = itrp.size()-2;i>0;i--)
    {
        int y = itrp[i].velocity;
        if (itrp[i].velocity < y) y = itrp[i].velocity;

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

