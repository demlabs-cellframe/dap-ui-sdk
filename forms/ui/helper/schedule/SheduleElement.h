#ifndef PAIR_H
#define PAIR_H

#include <QTime>

class SheduleElement
{
public:
    qreal time;
    qreal velocity;
    SheduleElement(qreal t_time, qreal t_velocity) :
        time(t_time), velocity(t_velocity) { }
};

#endif // PAIR_H
