#include "CustomLineEditBase.h"
#include <QDebug>
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
    hblLineEdit->addWidget(wgtMarginLeft,0);
    hblLineEdit->addWidget(lblIcon,1);
    hblLineEdit->addItem(spacer);
    hblLineEdit->addWidget(btnControl,2);
    hblLineEdit->addWidget(wgtMarginRight,3);

    hblLineEdit->setSpacing(0);
    hblLineEdit->setMargin(0);

    this->setLayout(hblLineEdit);
}

void CustomLineEditBase::setVisibleIcon(bool &a_visible)
{
    lblIcon->setVisible(a_visible);

}

void CustomLineEditBase::setVisibleButton(bool a_visible)
{
    btnControl->setVisible(a_visible);

}

void CustomLineEditBase::setMarginLeft(bool a_visible)
{
    wgtMarginLeft->setVisible(a_visible);

}

void CustomLineEditBase::setMarginRight(bool a_visible)
{
    wgtMarginRight->setVisible(a_visible);
}

bool CustomLineEditBase::event(QEvent * event)
{
    ///When you start typing , a button appears .
    if(event->type() == QEvent::InputMethod)
    {
        setVisibleButton(true);
        Utils::setPropertyAndUpdateStyle(this, Properties::NORMAL,false);
        Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
        //Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
    }

    ///If you lose focus if the text is there then the color changes to endEdit if empty then by default.
    if(event->type() == QEvent::FocusOut)
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

        if(!text().isEmpty())
        {
            Utils::setPropertyAndUpdateStyle(this, Properties::NORMAL,true);
            //Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"endEdit");
        }
        else
        {
            //Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
            Utils::setPropertyAndUpdateStyle(this, Properties::NORMAL,false);
        }
    }

    return QLineEdit::event(event);
}

void CustomLineEditBase::setWrongState(bool a_wrong)
{
        Utils::setPropertyAndUpdateStyle(this, Properties::WRONG,a_wrong);
}
