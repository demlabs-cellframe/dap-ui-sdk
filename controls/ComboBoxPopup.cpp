#include "ComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"

ComboBoxPopup::ComboBoxPopup(QWidget *a_parent)
    : CustomComboBoxPopup(a_parent)
{
    this->create<Ui::ComboBoxPopup>();
}

void ComboBoxPopup::setCaption(const QString &a_caption)
{
    this->setChildProperties(LBL_CAPTION_NAME, Properties::TEXT, a_caption);
}

QString ComboBoxPopup::listViewName()
{
    return LVW_LIST_NAME;
}

