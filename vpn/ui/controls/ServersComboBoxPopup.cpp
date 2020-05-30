#include "ServersComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"
#include <QScroller>
ServersComboBoxPopup::ServersComboBoxPopup(QMainWindow *a_parent)
    : CustomComboBoxPopup(a_parent)
{
    this->create<Ui::ComboBoxPopup>();
}

QString ServersComboBoxPopup::captionLabelName()
{
    return LBL_CAPTION_NAME;
}

QString ServersComboBoxPopup::listViewName()
{
    return LVW_LIST_NAME;
}
