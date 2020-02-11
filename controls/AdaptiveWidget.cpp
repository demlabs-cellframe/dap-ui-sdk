#include "AdaptiveWidget.h"

AdaptiveWidget::AdaptiveWidget(QWidget *a_parent /*= nullptr*/)
    :QStackedWidget(a_parent)
{

}

QWidget *AdaptiveWidget::variant(ScreenInfo::Rotation a_rotation /*= ScreenInfo::Rotation::Horizontal*/)
{
    return m_variants.value(a_rotation, nullptr);
}

