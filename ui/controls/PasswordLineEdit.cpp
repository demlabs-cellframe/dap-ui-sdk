#include "PasswordLineEdit.h"

PasswordLineEdit::PasswordLineEdit(QWidget *parent):CustomLineEditBase (parent)
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



