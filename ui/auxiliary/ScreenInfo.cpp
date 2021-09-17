#include "ScreenInfo.h"

ScreenInfo::Rotation ScreenInfo::s_currentRotation = ScreenInfo::Rotation::Vertical;

ScreenInfo::ScreenInfo()
{

}

QSize ScreenInfo::getWindowSizeInPoints()
{
    return QSize(428, 926);
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
