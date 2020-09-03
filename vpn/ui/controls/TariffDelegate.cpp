#include "TariffDelegate.h"
#include "TariffItem.h"

#include "defines.h"
#include "Utilz.h"

TariffDelegate::TariffDelegate(QWidget* a_parent)
    : CustomButtonDelegate(a_parent)
    , m_lblMonthPrice(new QLabel(this))
    , m_lblDuration(new QLabel(this))
{
    this             ->setObjectName("frmTariffDelegate");
    m_lblMonthPrice  ->setObjectName("lblMonthPrice");
    m_lblDuration->setObjectName("lblPlanDuration");

    this->layout()->addWidget(m_lblMonthPrice);
    this->layout()->addWidget(m_lblDuration);
    this->layout()->setMargin(0);
    this->layout()->setSpacing(0);
}

void TariffDelegate::setData(const QVariant &a_value, int role)
{
    if (role == Qt::UserRole && a_value.isValid() && a_value.canConvert<TariffItem>())
    {
        TariffItem tariff = a_value.value<TariffItem>();
        this->button()->setText(tariff.totalPriceText);
        m_lblDuration->setText(tariff.durationText);
        m_lblMonthPrice->setText(tariff.monthPriceText);
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
