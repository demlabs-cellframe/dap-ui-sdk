#include "ScreenWithComboBoxesAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

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
        ScreenComboBox* combo; Utils::findChild(a_widget, comboBoxName, combo);
        ScreenComboBoxPopup* popup = combo->popup();
        connect(popup, &ScreenComboBoxPopup::opened, this, &ScreenWithComboBoxesAbstract::comboBoxPopupOpened);
        connect(popup, &ScreenComboBoxPopup::closingStarted, this, &ScreenWithComboBoxesAbstract::hideComboBoxPopup);
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
            connect(a_popupScreen, &ComboBoxPopupScreen::resized, [popup] (const QSize &a_size)
            {
                popup->resize(a_size);
            });
        }

}
