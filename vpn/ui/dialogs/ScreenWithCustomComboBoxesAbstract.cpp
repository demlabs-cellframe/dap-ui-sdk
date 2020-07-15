#include "ScreenWithCustomComboBoxesAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

const QString ScreenWithCustomComboBoxesAbstract::SCREEN_NAME = "ScreenWithComboBoxesAbstract";

ScreenWithCustomComboBoxesAbstract::ScreenWithCustomComboBoxesAbstract(QWidget *a_parent /*= nullptr*/)
    :AdaptiveScreen(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

void ScreenWithCustomComboBoxesAbstract::initVariantUi(QWidget *a_widget)
{
    for (QString comboBoxName: this->comboBoxesNames())
    {
        ScreenComboBox* combo; Utils::findChild(a_widget, comboBoxName, combo);
        this->connectComboBox(combo);
    }
}

void ScreenWithCustomComboBoxesAbstract::connectComboBox(ComboBox *a_comboBox)
{
    ComboBoxPopup* popup = a_comboBox->popup();
    connect(popup, &CustomPopup::opened, this, &ScreenWithCustomComboBoxesAbstract::comboBoxPopupOpened);
    connect(popup, &CustomPopup::closed, this, &ScreenWithCustomComboBoxesAbstract::comboBoxPopupClosed);
}
