#include "schedule.h"

Schedule::Schedule() {
    m_elems.push_front(SheduleElement(time(nullptr),0));
}

void Schedule::addElem(quint64 newQuantity)
{
    time_t newTime = time(nullptr);
    int velocity;

    if (newTime == s_time) {
        velocity = diff = diff + static_cast<int>(m_elems.size()<=1? 0: newQuantity - s_quantity);

        m_elems.pop_front();
    } else {
        diff     = static_cast<int>(newQuantity - s_quantity);
        velocity = diff/(newTime - s_time);
        s_time   = newTime;
    }

    m_elems.push_front(SheduleElement(newTime, velocity));
    s_quantity = newQuantity;

    if (m_elems.size() > 40) {
        m_elems.pop_back();
    }
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

    // set to initial positions
    path.moveTo(
        width,
        y_shift(m_elems.begin()->velocity, height, maxVal)
    );

    int time_pos = size_of_chart - 2;
    // draw the path

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

    scene->addPath(path, pen, color);
}

