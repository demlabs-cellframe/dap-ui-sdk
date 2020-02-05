#include "AdaptiveWidget.h"

AdaptiveWidget::AdaptiveWidget(QWidget *parent /*= nullptr*/)
{

}

QWidget *AdaptiveWidget::variant(ScreenInfo::Rotation a_rotation /*= ScreenInfo::Rotation::Horizontal*/)
{
    return m_variants.value(a_rotation, nullptr);
}

