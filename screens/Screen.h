#ifndef SCREEN_H
#define SCREEN_H

#include "AdaptiveWidget.h"

class Screen : public AdaptiveWidget
{
public:
    Screen(QWidget *a_parent = nullptr);

//    virtual void activateScreen() = 0;
//    virtual QString screenName() = 0;

    virtual void initUi(QWidget * a_widget, ScreenInfo::Rotation a_rotation = ScreenInfo::Rotation::Horizontal) = 0;
};

#endif // SCREEN_H
