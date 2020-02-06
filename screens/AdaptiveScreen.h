#ifndef ADAPTIVE_SCREEN_H
#define ADAPTIVE_SCREEN_H

#include "AdaptiveWidget.h"

class AdaptiveScreen : public AdaptiveWidget
{
public:
    AdaptiveScreen(QWidget *a_parent = nullptr);

//    virtual void activateScreen() = 0;
//    virtual QString screenName() = 0;

    virtual void initVariantUi(QWidget * a_widget) = 0;
};

#endif // SCREEN_H
