#include "CustomLineEditBase.h"
#include <QDebug>
CustomLineEditBase::CustomLineEditBase(QWidget* parent):QLineEdit (parent)
{
    m_btnControl = new QPushButton(this);
    m_lblIcon = new QLabel(this);

    m_btnControl->setObjectName("control");
    m_lblIcon->setObjectName("image");

    m_btnControl->setVisible(false);
    m_lblIcon->setVisible(false);

    m_hblLineEdit = new QHBoxLayout(this);
    m_hblLineEdit->addWidget(m_lblIcon,0,Qt::AlignLeft);
    m_hblLineEdit->addWidget(m_btnControl,1,Qt::AlignRight);

    m_hblLineEdit->setSpacing(0);
    m_hblLineEdit->setMargin(0);

    this->setLayout(m_hblLineEdit);

}

void CustomLineEditBase::setVisibleIcon(bool &a_visible)
{
    m_lblIcon->setVisible(a_visible);

    int a_left,a_top,a_right,a_bottom;
    this->getTextMargins(&a_left,&a_top,&a_right,&a_bottom);

    this->setTextMargins(m_lblIcon->geometry().width(),a_top,a_right,a_bottom);
}

void CustomLineEditBase::setVisibleButton(bool a_visible)
{
    m_btnControl->setVisible(a_visible);

    int a_left,a_top,a_right,a_bottom;
    this->getTextMargins(&a_left,&a_top,&a_right,&a_bottom);

    this->setTextMargins(a_left,a_top,m_btnControl->geometry().width(),a_bottom);
}


