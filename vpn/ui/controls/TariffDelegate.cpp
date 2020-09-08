#include "TariffDelegate.h"

#include <QVBoxLayout>

#include "TariffItem.h"
#include "defines.h"
#include "Utilz.h"

TariffDelegate::TariffDelegate(QWidget* a_parent)
    : CustomButtonDelegate(a_parent)
    , m_doubledSubcontroll(new QWidget(this))
    , m_lblMonthPrice(new QLabel(this))
    , m_lblDuration(new QLabel(this))
{
    this             ->setObjectName("frmTariffDelegate");
    button()->setObjectName("btnTarif");
    m_lblMonthPrice  ->setObjectName("lblMonthPrice");
    m_lblDuration    ->setObjectName("lblPlanDuration");
    m_doubledSubcontroll ->setObjectName("doubledSubcontroll");


    QVBoxLayout* subcontrollLayout = new QVBoxLayout(m_doubledSubcontroll);
    m_doubledSubcontroll->setLayout(subcontrollLayout);
    subcontrollLayout->setMargin(0);
    subcontrollLayout->setSpacing(0);

    subcontrollLayout->addWidget(m_lblDuration);
    subcontrollLayout->addWidget(m_lblMonthPrice);


    this->button()->addSubcontrol(*m_doubledSubcontroll);
}

void TariffDelegate::setData(const QVariant &a_value, int role)
{
    if (role == Qt::UserRole && a_value.isValid() && a_value.canConvert<TariffItem>())
    {
        TariffItem tariff = a_value.value<TariffItem>();
        this->button()->setText(tariff.totalPriceText);
        m_lblDuration->setText(tariff.durationText);
        if(!tariff.monthPriceText.isEmpty())
        {
            m_lblMonthPrice->setText(tariff.monthPriceText);
        }
        else
        {
            m_lblMonthPrice->hide();
        }
    }
}

void TariffDelegate::setSelected(bool a_selected)
{
    this->CustomButtonDelegate::setSelected(a_selected);

    Utils::setPropertyAndUpdateStyle(m_lblDuration  , Properties::SELECTED, a_selected);
    Utils::setPropertyAndUpdateStyle(m_lblMonthPrice, Properties::SELECTED, a_selected);
}

WidgetDelegateBase *TariffDelegate::create()
{
    return new TariffDelegate;
}
