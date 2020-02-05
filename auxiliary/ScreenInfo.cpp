#include "ScreenInfo.h"

ScreenInfo::Rotation ScreenInfo::s_currentRotation = ScreenInfo::Rotation::Horizontal;

ScreenInfo::ScreenInfo()
{

}

QSize ScreenInfo::getWindowSizeInPoints()
{
    return QSize(1024, 744);
}

QList<ScreenInfo::Rotation> ScreenInfo::allRotations()
{
    return  { Rotation::Horizontal, Rotation::Vertical };
}

ScreenInfo::Rotation ScreenInfo::currentRotation()
{
    return ScreenInfo::s_currentRotation;
}

void ScreenInfo::setCurrentRotation(const Rotation &value)
{
    s_currentRotation = value;
}
