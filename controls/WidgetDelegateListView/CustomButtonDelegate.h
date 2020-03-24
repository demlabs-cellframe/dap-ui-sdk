#ifndef CUSTOMBUTTONDELEGATE_H
#define CUSTOMBUTTONDELEGATE_H

#include "WidgetDelegateBase.h"
#include "CustomPlacementButton.h"

class CustomButtonDelegate : public WidgetDelegateBase
{

public:
    CustomButtonDelegate(QWidget* a_parent = Q_NULLPTR);

    virtual void setData(const QVariant &value, int role = Qt::DisplayRole) override;

//    bool selected() const;
    virtual void setSelected(bool a_selected) override;

private:
    QWidget *m_topMargin;
    CustomPlacementButton* m_button;
    QWidget *m_bottomMargin;
//    bool m_selected;

};

#endif // CUSTOMBUTTONDELEGATE_H
