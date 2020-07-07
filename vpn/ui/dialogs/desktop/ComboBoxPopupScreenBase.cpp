#include "ComboBoxPopupScreenBase.h"

ComboBoxPopupScreenBase::ComboBoxPopupScreenBase(QWidget *a_parent /*= nullptr*/)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName("ComboBoxPopupScreen");

}

void ComboBoxPopupScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}

void ComboBoxPopupScreenBase::resizeEvent(QResizeEvent *event)
{
    this->AdaptiveScreen::resizeEvent(event);
    emit this->resized(this->size());
}
