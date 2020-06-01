#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "CustomButtonComboBox.h"
#include "ComboBoxPopup.h"

class ComboBox : public CustomButtonComboBox
{
    Q_OBJECT
public:
    ComboBox(QWidget* a_parent = nullptr);
    ComboBox(CustomComboBoxPopup* a_popup, QWidget* a_parent = nullptr);

    ComboBoxPopup* popup();

    CustomPlacementButton *buttonControll() const;

signals:
    void currentIndexChanged(const QString &); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
};

#endif // SERVERSCOMBOBOX_H
