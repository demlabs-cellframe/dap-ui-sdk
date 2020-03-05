#ifndef CUSTOMPLACEMENTBUTTONDELEGATEFACTORY_H
#define CUSTOMPLACEMENTBUTTONDELEGATEFACTORY_H

#include "WidgetDelegateFactoryBase.h"
#include "CustomButtonDelegate.h"

class CustomButtonDelegateFactory : public WidgetDelegateFactoryBase
{
public:
    virtual ~CustomButtonDelegateFactory(){};
    virtual WidgetDelegateBase* createWidgetDelegate(QWidget* a_parent = Q_NULLPTR)
    {
        return new CustomButtonDelegate(a_parent);
    }
};

#endif // CUSTOMPLACEMENTBUTTONDELEGATEFACTORY_H
