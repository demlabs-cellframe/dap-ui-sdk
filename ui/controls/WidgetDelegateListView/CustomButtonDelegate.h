#ifndef CUSTOMBUTTONDELEGATE_H
#define CUSTOMBUTTONDELEGATE_H

#include "WidgetDelegateBase.h"
#include "CustomPlacementButton.h"

class CustomButtonDelegate : public WidgetDelegateBase
{

public:
    CustomButtonDelegate(QWidget* a_parent = Q_NULLPTR);

    virtual void setData(const QVariant &value, int role = Qt::DisplayRole) override;

    CustomPlacementButton* button();

    virtual void setSelected(bool a_selected) override;

    static WidgetDelegateBase* create();

private:
    CustomPlacementButton* m_button;
};

#endif // CUSTOMBUTTONDELEGATE_H
