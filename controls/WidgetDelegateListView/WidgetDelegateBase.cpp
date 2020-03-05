#include "WidgetDelegateBase.h"

#include<QVariant>

#include "defines.h"
#include "Utils.h"
#include <QResizeEvent>

WidgetDelegateBase::WidgetDelegateBase(QWidget *parent)
    : QWidget(parent)
    , m_layout(new QBoxLayout(QBoxLayout::TopToBottom, this))
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    this->setLayout(m_layout);

}

void WidgetDelegateBase::setData(const QMap<int, QVariant> &a_dataMap)
{
    for (auto role: a_dataMap.keys())
        this->setData(a_dataMap.value(role), role);
}

void WidgetDelegateBase::setSelected(bool a_selected)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::SELECTED, a_selected);
}

void WidgetDelegateBase::resizeEvent(QResizeEvent *event)
{
    emit this->sizeChanged(this->size());

    this->QWidget::resizeEvent(event);
}
