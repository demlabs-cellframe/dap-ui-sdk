#include "schedule.h"

// return current time in ms
quint64 CurrentMillisecond()
{
    quint64 ctime = (quint64) time(nullptr);
    QTime qttime = QTime().currentTime();
    return ctime * 1000 + qttime.msec();
}


Schedule::Schedule()
{
  m_elems.push_front(SheduleElement (time (nullptr), 0));
}

void Schedule::addElem(quint64 newQuantity)
{
    s_sample0 = newQuantity;
}

void Schedule::updateSample()
{
    quint64 newTime = CurrentMillisecond();
    if (newTime != s_time)
    {
        viewfilter.push(1000 * (s_sample0 - s_sample1)/(newTime - s_time));
        m_elems.push_front(SheduleElement(newTime, viewfilter.sum()));
        s_sample1 = s_sample0;
        s_time = newTime;
        if (m_elems.size() > 40)
        {
            m_elems.pop_back();
        }
    }
}

void Schedule::reset()
{
  m_elems.clear();
  m_elems.push_front(SheduleElement (time (nullptr), 0));
  s_time = CurrentMillisecond();
  s_sample0 = 0;
  s_sample1 = 0;
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

int x_shift(int x, int width, int number_of_elem)
{
    if (number_of_elem == 0) {
        number_of_elem = 1;
    }

    int res = x * width / number_of_elem;
    return res;
}

int y_shift(int y, int height, int maxValue)
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
    int size_of_chart = size();
    QPainterPath path = QPainterPath();
    path.moveTo(
                width,
                y_shift(m_elems.begin()->velocity, height, maxVal)
                );
    int time_pos = size_of_chart - 2;
    for(int i = 1;i<m_elems.size();i++)
    {
        int y = 0;
        if (m_elems[i].velocity > y) y = m_elems[i].velocity;

        path.lineTo(
                    x_shift(time_pos, width, size_of_chart),
                    y_shift(y, height, maxVal));
        time_pos--;
    }
    time_pos = 1;
    for(int i = m_elems.size()-2;i>0;i--)
    {
        int y = m_elems[i].velocity;
        if (m_elems[i].velocity < y) y = m_elems[i].velocity;

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

