#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent):CustomLineEditBase (parent)
{


}

void CustomLineEdit::focusInEvent(QFocusEvent *e)
{
    setVisibleButton(true);
    CustomLineEditBase::focusInEvent(e);
}

void CustomLineEdit::focusOutEvent(QFocusEvent *e)
{
    if(m_btnControl!=Q_NULLPTR)
    if(!m_btnControl->hasFocus())
        setVisibleButton(false);

    CustomLineEditBase::focusOutEvent(e);
}

void CustomLineEdit::setSettingsButton()
{
    connect(m_btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
        this->setFocus();
        emit this->textEdited(this->text());
    });
}
