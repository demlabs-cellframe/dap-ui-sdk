#include "CustomLineEditBase.h"

CustomLineEditBase::CustomLineEditBase(QWidget* parent):QLineEdit (parent)
{
    wgtMarginLeft = new QWidget(this);
    wgtMarginLeft->setObjectName("wgtMarginLeft");
    wgtMarginLeft->setVisible(false);

    wgtMarginRight = new QWidget(this);
    wgtMarginRight->setObjectName("wgtMarginRight");
    wgtMarginRight->setVisible(false);

    spacer = new QSpacerItem(16000,10);

    btnControl = new QPushButton(this);
    lblIcon = new QLabel(this);

    btnControl->setObjectName("control");
    lblIcon->setObjectName("image");

    btnControl->setVisible(false);
    lblIcon->setVisible(false);

    hblLineEdit = new QHBoxLayout(this);
    hblLineEdit->addWidget(wgtMarginLeft,0,Qt::AlignLeft);
    hblLineEdit->addWidget(lblIcon,1,Qt::AlignLeft);
    hblLineEdit->addItem(spacer);
    hblLineEdit->addWidget(btnControl,2,Qt::AlignRight);
    hblLineEdit->addWidget(wgtMarginRight,3,Qt::AlignRight);

    hblLineEdit->setSpacing(0);
    hblLineEdit->setMargin(0);

    this->setLayout(hblLineEdit);

}

void CustomLineEditBase::setVisibleIcon(bool &a_visible)
{
    lblIcon->setVisible(a_visible);

    int wgtWidht = 0;
    if(wgtMarginLeft->isVisible())
    {
        wgtWidht = wgtMarginLeft->geometry().width();
    }

    int a_left,a_top,a_right,a_bottom;
    this->getTextMargins(&a_left,&a_top,&a_right,&a_bottom);

    this->setTextMargins(lblIcon->geometry().width() + wgtWidht,a_top,a_right,a_bottom);
}

void CustomLineEditBase::setVisibleButton(bool a_visible)
{
    btnControl->setVisible(a_visible);

    int wgtWidht = 0;
    if(wgtMarginRight->isVisible())
    {
        wgtWidht = wgtMarginRight->geometry().width();
    }

    int a_left,a_top,a_right,a_bottom;
    this->getTextMargins(&a_left,&a_top,&a_right,&a_bottom);

    this->setTextMargins(a_left,a_top,btnControl->geometry().width() + wgtWidht,a_bottom);
}

void CustomLineEditBase::setMarginLeft(bool a_visible)
{
    wgtMarginLeft->setVisible(a_visible);
}

void CustomLineEditBase::setMarginRight(bool a_visible)
{
    wgtMarginRight->setVisible(a_visible);
}


