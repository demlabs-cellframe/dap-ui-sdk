#include "ComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"
#include <QScroller>
ComboBoxPopup::ComboBoxPopup(QMainWindow *a_parent)
    : CustomComboBoxPopup(a_parent)
{
    this->create<Ui::ComboBoxPopup>();
}

QString ComboBoxPopup::captionLabelName()
{
    return LBL_CAPTION_NAME;
}

QString ComboBoxPopup::listViewName()
{
    return LVW_LIST_NAME;
}

#ifndef ANDROID
void ComboBoxPopup::showEvent(QShowEvent *event)
{
    m_canBeHidden = false;

    this->CustomComboBoxPopup::showEvent(event);
}

void ComboBoxPopup::setVisible(bool a_visible)
{
    if (!a_visible && !m_canBeHidden)
        emit closingStarted();

    else
        this->CustomComboBoxPopup::setVisible(a_visible);
}

void ComboBoxPopup::allowClosingAndHide()
{
    m_canBeHidden = true;
    this->hide();
}
#endif

