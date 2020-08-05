#include "PopupScreenBase.h"

PopupScreenBase::PopupScreenBase(QWidget *a_parent /*= nullptr*/)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName("ComboBoxPopupScreen");

}

void PopupScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}

void PopupScreenBase::resizeEvent(QResizeEvent *event)
{
    this->AdaptiveScreen::resizeEvent(event);
    emit this->resized(this->size());
}
