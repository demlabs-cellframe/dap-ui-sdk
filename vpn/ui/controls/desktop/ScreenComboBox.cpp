#include "ScreenComboBox.h"

#include "CustomPlacementButton.h"
#include "ScreenComboBoxPopup.h"

ScreenComboBox::ScreenComboBox(QWidget *a_parent)
    :ComboBox(new ScreenComboBoxPopup(a_parent), a_parent)
{
}

ScreenComboBoxPopup *ScreenComboBox::popup()
{
    return qobject_cast<ScreenComboBoxPopup*>(this->CustomButtonComboBox::popup());
}
