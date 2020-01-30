#include "UiScaling.h"

#include <QGuiApplication>
#include <QScreen>


int UiScaling::pointsToPixels(float a_pointsValue)
{
    static auto dpi(QGuiApplication::primaryScreen()->physicalDotsPerInch());

    return /*qRound*/static_cast<int>((dpi * pointsToInches(a_pointsValue)));
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
