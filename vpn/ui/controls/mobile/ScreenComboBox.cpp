#include "ScreenComboBox.h"

#include "CustomPlacementButton.h"
#include "ScreenComboBoxPopup.h"

ScreenComboBox::ScreenComboBox(QWidget *a_parent)
    :ComboBox(a_parent)
{
    this->popup()->setObjectName("ScreenComboBoxPopup");
    this->popup()->setWindowType(Qt::Popup);
    this->popup()->setSizePolicy(QSizePolicy::Fixed, this->popup()->sizePolicy().horizontalPolicy());
}
