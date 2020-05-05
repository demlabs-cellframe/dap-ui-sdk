#include "CustomTextEdit.h"


CustomTextEdit::CustomTextEdit(QWidget *a_parent)
    :QTextEdit (a_parent)
{
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
}

void CustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);

    QTextEdit::focusOutEvent(e);
}

void CustomTextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    if(this->toPlainText().isEmpty())
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
    else
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,true);

    QTextEdit::inputMethodEvent(event);
}

void CustomTextEdit::focusInEvent(QFocusEvent *e)
{

    Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,true);
    QTextEdit::focusInEvent(e);
}
