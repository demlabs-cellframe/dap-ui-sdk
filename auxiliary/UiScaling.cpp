#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>


int UiScaling::pointsToPixels(float a_points)
{
    static auto dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());
    static auto pixelsInPoint(devicePointSize() * dpi);
    return qRound(a_points * pixelsInPoint);
}

QSize UiScaling::pointsToPixels(const QSize &a_pointsSize)
{
    return QSize(UiScaling::pointsToPixels(a_pointsSize.width()),
                 UiScaling::pointsToPixels(a_pointsSize.height()));
}

inline float UiScaling::devicePointSize()
{
    return 1/160.f;
}
