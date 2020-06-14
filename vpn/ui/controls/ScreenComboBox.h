#ifndef SCREENCOMBOBOX_H
#define SCREENCOMBOBOX_H

#include "CustomButtonComboBox.h"
#include "ScreenComboBoxPopup.h"
#include "ComboBox.h"

/** @brief Combobox wich open popup in another screen
 *  @details Use ScreenComboBoxPopup as popup and CustomPlacementBuuton for comboBox controll
*/
class ScreenComboBox : public ComboBox
{
    Q_OBJECT
public:
    ScreenComboBox(QWidget* a_parent = nullptr);

    ScreenComboBoxPopup* popup();
};

#endif // SCREENCOMBOBOX_H
