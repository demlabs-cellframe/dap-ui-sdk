#include "CustomButtonDelegate.h"

#include "defines.h"
#include "Utils.h"

CustomButtonDelegate::CustomButtonDelegate(QWidget* a_parent)
    : WidgetDelegateBase(a_parent)
    , m_button(new CustomPlacementButton(this))
{
    this          ->setObjectName("frmCustomButtonDelegate");
    m_button      ->setObjectName("btnCustomButton");

    this->layout()->addWidget(m_button);
    layout()->setMargin(0);
    layout()->setSpacing(0);

    connect(m_button, &QPushButton::clicked, [this]{
        emit this->selected();
    });
}

void CustomButtonDelegate::setData(const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
        m_button->setText(value.toString());
        break;
    case Qt::DecorationRole:
        m_button->setIcon(value.toString());
        break;
    default:
        qDebug() << "Wrong role";
        break;
    }
}

CustomPlacementButton *CustomButtonDelegate::button()
{
    return m_button;
}

void CustomButtonDelegate::setSelected(bool a_selected)
{
    this->WidgetDelegateBase::setSelected(a_selected);

    Utils::setPropertyAndUpdateStyle(m_button, Properties::SELECTED, a_selected);
}
