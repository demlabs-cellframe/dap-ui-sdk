#ifndef WIDGETDELEGATEFACTORYBASE_H
#define WIDGETDELEGATEFACTORYBASE_H

class WidgetDelegateFactoryBase;
#include "WidgetDelegateBase.h"

class WidgetDelegateFactoryBase
{
public:
    virtual ~WidgetDelegateFactoryBase(){};

    virtual WidgetDelegateBase* createWidgetDelegate(QWidget* a_parent = Q_NULLPTR) = 0;
};

#endif // WIDGETDELEGATEFACTORYBASE_H
