#include "ServersComboBox.h"

#include "CustomPlacementButton.h"
#include "ServersComboBoxPopup.h"

ServersComboBox::ServersComboBox(QWidget *a_parent)
    : CustomButtonComboBox(a_parent)
{
    CustomPlacementButton *button = new CustomPlacementButton(this);

    this->setButtonControll(button);

    QMainWindow* mainWindow = Utils::findParent<QMainWindow*>(a_parent);
    this->setPopup(new ServersComboBoxPopup(mainWindow));
}

ServersComboBoxPopup *ServersComboBox::popup()
{
    return qobject_cast<ServersComboBoxPopup*>(this->CustomButtonComboBox::popup());
}

CustomPlacementButton *ServersComboBox::buttonControll() const
{
    return qobject_cast<CustomPlacementButton*>(CustomButtonComboBox::buttonControll());
}
