#include "ComboBox.h"

#include "CustomPlacementButton.h"

ComboBox::ComboBox(QWidget *a_parent)
    : CustomButtonComboBox(a_parent)
{
    CustomPlacementButton *button = new CustomPlacementButton(this);

    this->setButtonControll(button);

    this->setGraphicsEffect(new StyledDropShadowEffect(button));
}

void ComboBox::setObjectName(const QString &a_name)
{
    this->CustomButtonComboBox::setObjectName(a_name);

    CustomPlacementButton *button = this->buttonControll();

    if (button)
        button->setObjectName(button->objectName()); //Needed for shadow updating

    if (m_styledShadow)
        m_styledShadow->updateStyleProperties();
}

void ComboBox::setGraphicsEffect(StyledDropShadowEffect *a_effect)
{
    m_styledShadow = a_effect;
    CustomButtonComboBox::setGraphicsEffect(a_effect);
}

CustomPlacementButton *ComboBox::buttonControll() const
{
    return qobject_cast<CustomPlacementButton*>(CustomButtonComboBox::buttonControll());
}
