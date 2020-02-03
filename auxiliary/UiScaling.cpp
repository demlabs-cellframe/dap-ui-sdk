#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>


float UiScaling::pointsToPixels(float a_pointsValue)
{
    static qreal dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());

    float valueInPixels = dpi * pointsToInches(a_pointsValue);

    return valueInPixels;
}

QSize UiScaling::pointsToPixels(const QSize &a_pointsSize)
{
    return QSize(UiScaling::pointsToPixels(a_pointsSize.width()),
                 UiScaling::pointsToPixels(a_pointsSize.height()));
}

inline double UiScaling::pointsToInches(float a_pointsValue)
{
    return a_pointsValue / 163.f;
}
