#include "ScreenComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"
#include <QScroller>


ScreenComboBoxPopup::ScreenComboBoxPopup(QWidget *a_parent)
    : ComboBoxPopup(a_parent)
{
#ifdef ANDROID
    this->setObjectName("ScreenComboBoxPopup");
    this->setWindowType(Qt::Popup);

     //remove widgets from items in all listViews (not one because adaptive)
    for (QListView* l_listView: this->allListViews())
    {
        WidgetDelegateListView* l_customListView = qobject_cast<WidgetDelegateListView*>(l_listView);
        if (l_customListView)
            l_customListView->setWidgetDelegateFactory(nullptr);
    }
#endif
    this->setChildProperties(LBL_CAPTION_NAME, Properties::VISIBLE, true);

}

void ScreenComboBoxPopup::initVariantUi(QWidget *a_widget)
{
    this->CustomComboBoxPopup::initVariantUi(a_widget);
}

#ifndef ANDROID
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
#endif
