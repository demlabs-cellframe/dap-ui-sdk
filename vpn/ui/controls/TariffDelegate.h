#ifndef TARIFFDELEGATE_H
#define TARIFFDELEGATE_H

#include "CustomButtonDelegate.h"

class TariffDelegate : public CustomButtonDelegate
{
    Q_OBJECT
public:
    TariffDelegate(QWidget* a_parent = Q_NULLPTR);

    virtual void setData(const QVariant &a_value, int role = Qt::DisplayRole) override;

    virtual void setSelected(bool a_selected) override;

    static WidgetDelegateBase* create();

private:
    QWidget* m_doubledSubcontroll;
    QLabel* m_lblMonthPrice;
    QLabel* m_lblDuration;
};


class TariffSubcontroll:public QWidget
{

};

#endif // CUSTOMBUTTONDELEGATE_H
