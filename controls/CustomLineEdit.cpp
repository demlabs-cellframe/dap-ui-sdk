#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent):CustomLineEditBase (parent)
{

    connect(btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
        Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"");
    });
}

    void CustomLineEdit::setErrorLine(bool a_error)
    {
        if(a_error)
        {
            Utils::setPropertyAndUpdateStyle(this, Properties::STATE,"error");
            Utils::setPropertyAndUpdateStyle(btnControl, Properties::STATE,"error");
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

            Utils::setPropertyAndUpdateStyle(btnControl, Properties::STATE,"");

        }
    }

bool CustomLineEdit::event(QEvent * event)
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
