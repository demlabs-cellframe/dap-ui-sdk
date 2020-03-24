#include "CustomPasswordLineEdit.h"

CustomPasswordLineEdit::CustomPasswordLineEdit(QWidget *parent):CustomLineEditBase (parent)
{
    btnControl->setCheckable(true);


    connect(btnControl,&QPushButton::clicked,[=]{
        if(btnControl->isChecked())
        {
            btnControl->setChecked(true);
            this->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            btnControl->setChecked(false);
            this->setEchoMode(QLineEdit::Password);
        }
    });
}

    void CustomPasswordLineEdit::setErrorLine(bool a_error)
    {
        if(a_error)
        {
            Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"error");
        }
        else
        {
            if(!text().isEmpty())
            {
                Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"endEdit");
            }
            else
            {
                Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
            }
        }
    }

bool CustomPasswordLineEdit::event(QEvent * event)
{
    ///When you start typing , a button appears .
    if(event->type() == QEvent::InputMethod)
    {
        setVisibleButton(true);
        Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
    }
    ///If you lose focus if the text is there then the color changes to endEdit if empty then by default.
    if(event->type() == QEvent::FocusOut)
    {
        if(!text().isEmpty())
        {
            Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"endEdit");
        }
        else
        {
            Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
        }
    }
    return QLineEdit::event(event);
}
