#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent):CustomLineEditBase (parent)
{
    connect(m_btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
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
    if(!m_btnControl->hasFocus())
        setVisibleButton(false);

    CustomLineEditBase::focusOutEvent(e);
}
