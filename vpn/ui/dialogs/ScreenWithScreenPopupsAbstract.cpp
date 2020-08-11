#include "ScreenWithScreenPopupsAbstract.h"

#include "ScreenComboBox.h"
#include "ScreenComboBoxPopup.h"

const QString ScreenWithScreenPopupsAbstract::SCREEN_NAME = "ScreenWithScreenPopupsAbstract";

ScreenWithScreenPopupsAbstract::ScreenWithScreenPopupsAbstract(QWidget *a_parent /*= nullptr*/)
    :ScreenWithCustomPopupsAbstract(a_parent)
{
    AdaptiveScreen::initScreen(this);
}

void ScreenWithScreenPopupsAbstract::initVariantUi(QWidget *a_widget)
{
    this->ScreenWithCustomPopupsAbstract::initVariantUi(a_widget);
}

void ScreenWithScreenPopupsAbstract::setPopupScreen(PopupScreenBase* a_popupScreen)
{
    for (CustomPopup* popup: this->customPopups())
    {
        popup->setParent(a_popupScreen);
        popup->setWindowType(Qt::Desktop);

        ScreenComboBoxPopup *screenPopup = qobject_cast<ScreenComboBoxPopup*>(popup);
        if (screenPopup)
            connect(a_popupScreen, &PopupScreenBase::hidden, screenPopup, &ScreenComboBoxPopup::allowClosingAndHide);


        connect(a_popupScreen, &PopupScreenBase::resized, [popup] (const QSize &a_size)
        {
            popup->resize(a_size);
        });
    }
}

void ScreenWithScreenPopupsAbstract::connectPopup(CustomPopup *a_popup)
{
    ScreenComboBoxPopup* screenPopup = qobject_cast<ScreenComboBoxPopup*>(a_popup);

    if (!screenPopup)
        return this->ScreenWithCustomPopupsAbstract::connectPopup(a_popup);

    connect(screenPopup, &ScreenComboBoxPopup::closingStarted, this, &ScreenWithScreenPopupsAbstract::popupClosingStarted);
    connect(screenPopup, &CustomPopup::opened                , this, &ScreenWithScreenPopupsAbstract::screenComboBoxOpened);
    connect(screenPopup, &CustomPopup::opened                , screenPopup, [this, screenPopup]
    {
        emit this->showedScreenPopupCaptionChanged(screenPopup->caption());
    });


}
