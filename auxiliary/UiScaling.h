#ifndef UISCALING_H
#define UISCALING_H

#include <QSize>

class UiScaling
{
    UiScaling();             ///< private! Can't be created. Only for static methods

public:

    static int pointsToPixels(float a_points);
    static QSize pointsToPixels(const QSize& a_pointsSize);

    inline static float devicePointSize();
};

#endif // UISCALING_H
