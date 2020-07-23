#include "PasswordLineEdit.h"

PasswordLineEdit::PasswordLineEdit(QWidget *parent):CustomLineEditBase (parent)
{
    m_btnControl->setCheckable(true);


    connect(m_btnControl,&QPushButton::clicked,[=]{
        if(m_btnControl->isChecked())
        {
            m_btnControl->setChecked(true);
            this->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            m_btnControl->setChecked(false);
            this->setEchoMode(QLineEdit::Password);
        }
    });
}



