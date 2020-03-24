#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "CustomButtonComboBox.h"

class ComboBox : public CustomButtonComboBox
{
public:
    ComboBox(QWidget* a_parent = nullptr);

    void setObjectName(const QString &a_name);

    void setGraphicsEffect(StyledDropShadowEffect *a_effect);

private:
    ///For effect.
    StyledDropShadowEffect *m_styledShadow = nullptr;


};

#endif // COMBOBOX_H
