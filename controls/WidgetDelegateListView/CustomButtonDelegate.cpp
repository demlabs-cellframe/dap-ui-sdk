#include "CustomButtonDelegate.h"

#include "defines.h"
#include "Utils.h"

CustomButtonDelegate::CustomButtonDelegate(QWidget* a_parent)
    : WidgetDelegateBase(a_parent)
    , m_topMargin(new QWidget(this))
    , m_button(new CustomPlacementButton(this))
    , m_bottomMargin(new QWidget(this))
{
 //   m_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this          ->setObjectName("wgtCustomButtonDelegate");
    m_topMargin   ->setObjectName("wgtTopMargin");
    m_button      ->setObjectName("btnCustomButton");
    m_bottomMargin->setObjectName("wgtBottomMargin");

    m_button->setGraphicsEffect(new StyledDropShadowEffect(m_button));

    this->layout()->addWidget(m_topMargin);
    this->layout()->addWidget(m_button);
    this->layout()->addWidget(m_bottomMargin);

    this->layout()->setAlignment(m_button, Qt::AlignHCenter);

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

//bool CustomPlacementButtonDelegate::selected() const
//{
//    return m_selected;
//}

void CustomButtonDelegate::setSelected(bool a_selected)
{
    this->WidgetDelegateBase::setSelected(a_selected);

    Utils::setPropertyAndUpdateStyle(m_button, Properties::SELECTED, a_selected);
}
