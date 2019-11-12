#ifndef PAIR_H
#define PAIR_H

class SheduleElement
{
public:
    int time;
    int velocity;
    SheduleElement(int t_time, int t_velocity) :
        time(t_time), velocity(t_velocity) { }
};

#endif // PAIR_H
