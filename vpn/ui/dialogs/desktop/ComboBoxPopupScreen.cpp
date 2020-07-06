#include "ComboBoxPopupScreen.h"

const QString ComboBoxPopupScreen::SCREEN_NAME = "ComboBoxPopup";

ComboBoxPopupScreen::ComboBoxPopupScreen(QWidget *a_parent /*= nullptr*/)
    : ComboBoxPopupScreenBase(a_parent)
{
    this->createEmpty();

    AdaptiveScreen::initScreen(this);
}

QString ComboBoxPopupScreen::screenName()
{
    return ComboBoxPopupScreen::SCREEN_NAME;
}
