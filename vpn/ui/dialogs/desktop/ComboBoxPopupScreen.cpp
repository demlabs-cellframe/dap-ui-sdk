#include "ComboBoxPopupScreen.h"

const QString ComboBoxPopupScreen::SCREEN_NAME = "ComboBoxPopup";

ComboBoxPopupScreen::ComboBoxPopupScreen(QWidget *a_parent /*= nullptr*/)
    : AdaptiveScreen(a_parent)
{
    this->createEmpty();
    this->setObjectName("ComboBoxPopupScreen");

    AdaptiveScreen::initScreen(this);
}

QString ComboBoxPopupScreen::screenName()
{
    return ComboBoxPopupScreen::SCREEN_NAME;
}

void ComboBoxPopupScreen::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}

void ComboBoxPopupScreen::resizeEvent(QResizeEvent *event)
{
    this->AdaptiveScreen::resizeEvent(event);
    emit this->resized(this->size());
}
