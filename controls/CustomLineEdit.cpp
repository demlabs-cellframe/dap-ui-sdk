#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent):CustomLineEditBase (parent)
{

    connect(btnControl,&QPushButton::clicked,[=]{
        this->clear();
        setVisibleButton(false);
        Utils::setPropertyAndUpdateStyle(this, Properties::FILLED,false);
        Utils::setPropertyAndUpdateStyle(this, Properties::ACTIVE,false);
    });
}

