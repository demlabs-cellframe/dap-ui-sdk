#include "ComboBox.h"

#include "CustomPlacementButton.h"
#include "ComboBoxPopup.h"

ComboBox::ComboBox(QWidget *a_parent)
    :ComboBox(new ComboBoxPopup(a_parent), a_parent)
{
}

ComboBox::ComboBox(CustomComboBoxPopup *a_popup, QWidget *a_parent /*= nullptr*/)
    : CustomButtonComboBox(a_parent)
{
    CustomPlacementButton *button = new CustomPlacementButton(this);
    QLabel *label = new QLabel(this);

    this->setButtonControll(button);
    this->setInscriptionComboBox(label);
    this->setPopup(a_popup);
}

ComboBoxPopup *ComboBox::popup()
{
    return qobject_cast<ComboBoxPopup*>(this->CustomButtonComboBox::popup());
}

CustomPlacementButton *ComboBox::buttonControll() const
{
    return qobject_cast<CustomPlacementButton*>(CustomButtonComboBox::buttonControll());
}

QLabel *ComboBox::buttonInscription() const
{
    return qobject_cast<QLabel*>(this->CustomButtonComboBox::buttonInscription());
}
