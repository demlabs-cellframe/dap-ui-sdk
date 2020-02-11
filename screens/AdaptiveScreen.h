#ifndef ADAPTIVE_SCREEN_H
#define ADAPTIVE_SCREEN_H

#include "AdaptiveWidget.h"

class AdaptiveScreen : public AdaptiveWidget
{
    Q_OBJECT
public:

    AdaptiveScreen(QWidget *a_parent = nullptr);

//    virtual void activateScreen() = 0;
    virtual QString screenName() = 0;

    virtual void initVariantUi(QWidget * a_widget) = 0;

protected:
    typedef void (*InitializerFunc)(AdaptiveScreen*);

    template<typename T>
    static void initScreen(T* a_screen){
        if (T::s_initializerFunc)
            T::s_initializerFunc(a_screen);
    };
    static InitializerFunc s_initializerFunc;

};

#endif // SCREEN_H
