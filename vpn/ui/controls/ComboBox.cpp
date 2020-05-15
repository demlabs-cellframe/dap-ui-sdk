#include "ComboBox.h"

#include "CustomPlacementButton.h"
#include "ComboBoxPopup.h"

ComboBox::ComboBox(QWidget *a_parent)
    : CustomButtonComboBox(a_parent)
{
    CustomPlacementButton *button = new CustomPlacementButton(this);

    this->setButtonControll(button);

    QMainWindow* mainWindow = Utils::findParent<QMainWindow*>(a_parent);
    this->setPopup(new ComboBoxPopup(mainWindow));
}

ComboBoxPopup *ComboBox::popup()
{
    return qobject_cast<ComboBoxPopup*>(this->CustomButtonComboBox::popup());
}

CustomPlacementButton *ComboBox::buttonControll() const
{
    return qobject_cast<CustomPlacementButton*>(CustomButtonComboBox::buttonControll());
}
