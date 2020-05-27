﻿#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent):CustomLineEditBase (parent)
{
    connect(btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
        this->setFocus();
    });
}

void CustomLineEdit::focusInEvent(QFocusEvent *e)
{
    setVisibleButton(true);
    CustomLineEditBase::focusInEvent(e);
}

void CustomLineEdit::focusOutEvent(QFocusEvent *e)
{
    if(!btnControl->hasFocus())
        setVisibleButton(false);

    CustomLineEditBase::focusOutEvent(e);
}
