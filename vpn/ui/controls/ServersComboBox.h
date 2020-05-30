#ifndef SERVERSCOMBOBOX_H
#define SERVERSCOMBOBOX_H

#include "CustomButtonComboBox.h"
#include "ServersComboBoxPopup.h"

class ServersComboBox : public CustomButtonComboBox
{
    Q_OBJECT
public:
    ServersComboBox(QWidget* a_parent = nullptr);

    ServersComboBoxPopup* popup();

    CustomPlacementButton *buttonControll() const;
};

#endif // SERVERSCOMBOBOX_H
