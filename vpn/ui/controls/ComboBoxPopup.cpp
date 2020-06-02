#include "ComboBoxPopup.h"

#include <QDebug>
#include "ui_ComboBoxPopup.h"

#include "defines.h"
#include "WidgetDelegateListView.h"
#include <QScroller>
ComboBoxPopup::ComboBoxPopup(QMainWindow *a_parent /*= nullptr*/)
    : CustomComboBoxPopup(a_parent)
{
    this->create<Ui::ComboBoxPopup>();
}

ComboBoxPopup::ComboBoxPopup(QWidget *a_parent /*= nullptr*/)
    :ComboBoxPopup(Utils::findParent<QMainWindow*>(a_parent))
{
}

QString ComboBoxPopup::captionLabelName()
{
    return LBL_CAPTION_NAME;
}

QString ComboBoxPopup::listViewName()
{
    return LVW_LIST_NAME;
}

void ComboBoxPopup::initVariantUi(QWidget *a_widget)
{
    QLabel* lblCaption; Utils::findChild(a_widget, "lblCaption", lblCaption);
    lblCaption->hide();

    this->CustomComboBoxPopup::initVariantUi(a_widget);
}
