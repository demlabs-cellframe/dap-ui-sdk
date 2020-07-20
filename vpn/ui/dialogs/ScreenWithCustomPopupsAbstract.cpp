#include "ScreenWithCustomPopupsAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

const QString ScreenWithCustomPopupsAbstract::SCREEN_NAME = "ScreenWithCustomPopupsAbstract";

ScreenWithCustomPopupsAbstract::ScreenWithCustomPopupsAbstract(QWidget *a_parent /*= nullptr*/)
    :AdaptiveScreen(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

void ScreenWithCustomPopupsAbstract::closePopups()
{
    for (CustomPopup* popup: this->customPopups())
    {
        popup->close();
    }
}

QString ScreenWithCustomPopupsAbstract::screenName()
{
    return ScreenWithCustomPopupsAbstract::SCREEN_NAME;
}

void ScreenWithCustomPopupsAbstract::initVariantUi(QWidget *a_widget)
{
    for (CustomPopup* popup: this->customPopups())
        this->connectPopup(popup);
}

void ScreenWithCustomPopupsAbstract::connectPopup(CustomPopup *a_popup)
{
//    ComboBoxPopup* popup = a_comboBox->popup();
    connect(a_popup, &CustomPopup::opened, this, &ScreenWithCustomPopupsAbstract::popupOpened);
    connect(a_popup, &CustomPopup::closed, this, &ScreenWithCustomPopupsAbstract::popupClosed);
}
