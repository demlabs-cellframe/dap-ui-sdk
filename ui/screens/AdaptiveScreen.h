#ifndef ADAPTIVE_SCREEN_H
#define ADAPTIVE_SCREEN_H

#include "AdaptiveWidget.h"
#include <QDebug>

class AdaptiveScreen : public AdaptiveWidget
{
    Q_OBJECT
public:

    AdaptiveScreen(QWidget *a_parent = nullptr);

    virtual QString screenName() = 0;

    virtual void initVariantUi(QWidget * a_widget) override = 0;


    template<typename T>
    static void setInitFunc(void (*a_initFunc)(T*));

protected:

    template<typename T>
    static void initScreen(T* a_screen);

private:

    static QMap<QString, void*> s_initFuncMap;
};



////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void AdaptiveScreen::setInitFunc(void (*a_initFunc)(T*)/*typename T::InitializerFunc a_initFunc*/) {
    s_initFuncMap[T::SCREEN_NAME] = reinterpret_cast<void*>(a_initFunc);
};

template<typename T>
void AdaptiveScreen::initScreen(T* a_screen) {
    void* l_initFunctionVoid = s_initFuncMap.value(T::SCREEN_NAME, nullptr);
    if (l_initFunctionVoid)
    {
        typedef void (*funcType)(T*);
        auto l_initFunction = reinterpret_cast<funcType>(l_initFunctionVoid);
        l_initFunction(a_screen);
    }
}

#endif // SCREEN_H
