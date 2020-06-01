#include "ScreenWithComboBoxesAbstract.h"

#include "ComboBox.h"
#include "ComboBoxPopup.h"

const QString ScreenWithComboBoxesAbstract::SCREEN_NAME = "ScreenWithComboBoxesAbstract";

ScreenWithComboBoxesAbstract::ScreenWithComboBoxesAbstract(QWidget *a_parent /*= nullptr*/)
    :AdaptiveScreen(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

void ScreenWithComboBoxesAbstract::initVariantUi(QWidget *a_widget)
{
    for (QString comboBoxName: this->comboBoxesNames())
    {
        ComboBox* combo; Utils::findChild(a_widget, comboBoxName, combo);
        ComboBoxPopup* popup = combo->popup();
        connect(popup, &ComboBoxPopup::opened, this, &ScreenWithComboBoxesAbstract::comboBoxPopupOpened);
        connect(popup, &ComboBoxPopup::closingStarted, this, &ScreenWithComboBoxesAbstract::hideComboBoxPopup);
    }
}

void ScreenWithComboBoxesAbstract::setComboBoxPopupScreen(ComboBoxPopupScreen* a_popupScreen)
{
    for (QString comboBoxName: this->comboBoxesNames())
        for (ComboBox* combo: this->getTheSameWidgets<ComboBox>(comboBoxName))
        {
            ComboBoxPopup* popup = combo->popup();
            if (!popup)
                return;

            popup->setParent(a_popupScreen);
            popup->setWindowType(Qt::Desktop);
            connect(a_popupScreen, SIGNAL(hidden()), popup, SLOT(allowClosingAndHide()));
        }
}
