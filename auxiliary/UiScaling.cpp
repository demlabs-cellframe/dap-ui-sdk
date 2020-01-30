#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>


int UiScaling::pointsToPixels(float a_pointsValue)
{
    static auto dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());

    int valueInPixels = static_cast<int>((dpi * pointsToInches(a_pointsValue)));
    return (valueInPixels == 0) ? 1 : valueInPixels;
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
