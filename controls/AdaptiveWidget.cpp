#include "AdaptiveWidget.h"

AdaptiveWidget::AdaptiveWidget(QWidget *a_parent /*= nullptr*/)
    :QStackedWidget(a_parent)
{

}

QWidget *AdaptiveWidget::variant(ScreenInfo::Rotation a_rotation /*= ScreenInfo::Rotation::Horizontal*/)
{
    return m_variants.value(a_rotation, nullptr);
}

void AdaptiveWidget::setChildProperties(const QString &a_objName, const QString &a_property, const QVariant &a_value)
{
    for (auto widget : getTheSameWidgets<QWidget>(a_objName)) {
        widget->setProperty(a_property.toLatin1().constData(), a_value);
    }
}

void AdaptiveWidget::updateChildStyle(const QString &a_objName)
{
    for (auto widget : getTheSameWidgets<QWidget>(a_objName)) {
        widget->style()->unpolish(widget);
        widget->style()->polish(widget);
    }
}

