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


