#include "ScreenWithCustomComboBoxesAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

const QString ScreenWithCustomComboBoxesAbstract::SCREEN_NAME = "ScreenWithCustomComboBoxesAbstract";

ScreenWithCustomComboBoxesAbstract::ScreenWithCustomComboBoxesAbstract(QWidget *a_parent /*= nullptr*/)
    :AdaptiveScreen(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

QString ScreenWithCustomComboBoxesAbstract::screenName()
{
    return ScreenWithCustomComboBoxesAbstract::SCREEN_NAME;
}

void ScreenWithCustomComboBoxesAbstract::initVariantUi(QWidget *a_widget)
{
    for (ComboBox* comboBox: this->customComboBoxes())
        this->connectComboBox(comboBox);
}

void ScreenWithCustomComboBoxesAbstract::connectComboBox(ComboBox *a_comboBox)
{
    ComboBoxPopup* popup = a_comboBox->popup();
    connect(popup, &CustomPopup::opened, this, &ScreenWithCustomComboBoxesAbstract::comboBoxPopupOpened);
    connect(popup, &CustomPopup::closed, this, &ScreenWithCustomComboBoxesAbstract::comboBoxPopupClosed);
}
