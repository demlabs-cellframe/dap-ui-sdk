#include "ScreenWithScreenComboBoxesAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

const QString ScreenWithScreenComboBoxesAbstract::SCREEN_NAME = "ScreenWithComboBoxesAbstract";

ScreenWithScreenComboBoxesAbstract::ScreenWithScreenComboBoxesAbstract(QWidget *a_parent /*= nullptr*/)
    :ScreenWithCustomComboBoxesAbstract(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

void ScreenWithScreenComboBoxesAbstract::initVariantUi(QWidget *a_widget)
{
    for (QString comboBoxName: this->comboBoxesNames())
    {
        ScreenComboBox* combo; Utils::findChild(a_widget, comboBoxName, combo);
        this->connectComboBox(combo);
    }
}

void ScreenWithScreenComboBoxesAbstract::setComboBoxPopupScreen(ComboBoxPopupScreenBase* a_popupScreen)
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
            connect(a_popupScreen, &ComboBoxPopupScreenBase::resized, [popup] (const QSize &a_size)
            {
                popup->resize(a_size);
            });
        }

}

void ScreenWithScreenComboBoxesAbstract::connectComboBox(ComboBox *a_comboBox)
{
    ScreenComboBox* screenComboBox = qobject_cast<ScreenComboBox*>(a_comboBox);
    if (screenComboBox)
    {
        ScreenComboBoxPopup* popup = screenComboBox->popup();
        connect(popup, &ScreenComboBoxPopup::closingStarted, this, &ScreenWithScreenComboBoxesAbstract::comboBoxPopupClosingStarted);
    }

    this->ScreenWithCustomComboBoxesAbstract::connectComboBox(a_comboBox);
}


