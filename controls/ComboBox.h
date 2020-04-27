#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "CustomButtonComboBox.h"

class ComboBox : public CustomButtonComboBox
{
    Q_OBJECT
public:
    ComboBox(QWidget* a_parent = nullptr);

    void setObjectName(const QString &a_name);

    CustomPlacementButton *buttonControll() const;

private:

};

#endif // COMBOBOX_H
