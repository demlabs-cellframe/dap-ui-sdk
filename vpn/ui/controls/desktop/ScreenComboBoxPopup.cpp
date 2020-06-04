#include "ScreenComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"
#include <QScroller>


ScreenComboBoxPopup::ScreenComboBoxPopup(QWidget *a_parent)
    : ComboBoxPopup(a_parent)
{
}

void ScreenComboBoxPopup::showEvent(QShowEvent *event)
{
    m_canBeHidden = false;

    this->CustomComboBoxPopup::showEvent(event);
}

void ScreenComboBoxPopup::setVisible(bool a_visible)
{
    if (!a_visible && !m_canBeHidden)
        emit closingStarted();

    else
        this->CustomComboBoxPopup::setVisible(a_visible);
}

void ScreenComboBoxPopup::allowClosingAndHide()
{
    m_canBeHidden = true;
    this->hide();
}

