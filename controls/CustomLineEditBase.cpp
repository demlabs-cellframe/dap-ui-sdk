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
    hblLineEdit->addWidget(wgtMarginLeft);
    hblLineEdit->addWidget(lblIcon);
    hblLineEdit->addItem(spacer);
    hblLineEdit->addWidget(btnControl);
    hblLineEdit->addWidget(wgtMarginRight);

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

void CustomLineEditBase::setWrongState(bool a_wrong)
{
        Utils::setPropertyAndUpdateStyle(this, Properties::WRONG,a_wrong);
}

void CustomLineEditBase::focusOutEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

    if(!text().isEmpty())
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,true);
    }
    else
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
    }

    QLineEdit::focusOutEvent(e);
}

void CustomLineEditBase::inputMethodEvent(QInputMethodEvent *event)
{
    setVisibleButton(true);
    Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
    QLineEdit::inputMethodEvent(event);
}
