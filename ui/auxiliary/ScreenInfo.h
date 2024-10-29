#ifndef SCREENSIZEINFO_H
#define SCREENSIZEINFO_H

#include <QObject>
#include <QSize>
#include <QSet>

class ScreenInfo
{
    Q_GADGET

    ScreenInfo();

public:
    enum class Rotation { Horizontal, Vertical }; Q_ENUM(Rotation);


    static QSize getWindowSizeInPoints();
    static QList<Rotation> allRotations();

    static Rotation currentRotation();
    static void setCurrentRotation(const Rotation &value);

private:
    static Rotation s_currentRotation;


};

Q_DECLARE_METATYPE(ScreenInfo::Rotation);

#endif // SCREENSIZEINFO_H
