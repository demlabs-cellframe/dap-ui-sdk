#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "CustomButtonComboBox.h"

class ComboBox : public CustomButtonComboBox
{
    Q_OBJECT
public:
    ComboBox(QWidget* a_parent = nullptr);

    CustomPlacementButton *buttonControll() const;
};

#endif // COMBOBOX_H
