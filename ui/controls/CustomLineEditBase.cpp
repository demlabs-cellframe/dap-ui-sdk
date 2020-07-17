#include "CustomLineEditBase.h"
#include <QDebug>
CustomLineEditBase::CustomLineEditBase(QWidget* parent):QLineEdit (parent)
{
    spacer = new QSpacerItem(16000,10);

    btnControl = new QPushButton(this);
    lblIcon = new QLabel(this);

    btnControl->setObjectName("control");
    lblIcon->setObjectName("image");

    btnControl->hide();
    lblIcon->hide();

    hblLineEdit = new QHBoxLayout(this);
    hblLineEdit->addWidget(lblIcon);
    hblLineEdit->addItem(spacer);
    hblLineEdit->addWidget(btnControl);

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

void CustomLineEditBase::setWrongState(bool a_wrong)
{
        Utils::setPropertyAndUpdateStyle(this, Properties::WRONG,a_wrong);
}

void CustomLineEditBase::focusOutEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

    if(text().isEmpty())
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
    }
    else
    {
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,true);
    }
    QLineEdit::focusOutEvent(e);
}

void CustomLineEditBase::focusInEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
    QLineEdit::focusInEvent(e);
}

