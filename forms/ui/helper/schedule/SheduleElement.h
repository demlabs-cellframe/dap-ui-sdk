#ifndef PAIR_H
#define PAIR_H

#include <QTime>

class SheduleElement
{
public:
    qint64 time;
    int velocity;
    SheduleElement(qint64 t_time, int t_velocity) :
        time(t_time), velocity(t_velocity) { }
};

#endif // PAIR_H
