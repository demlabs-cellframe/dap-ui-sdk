#ifndef COMBOBOXLISTVIEW_H
#define COMBOBOXLISTVIEW_H

#include "WidgetDelegateListView.h"


class ComboBoxListView: public WidgetDelegateListView
{
    Q_OBJECT
public:
    ComboBoxListView(QWidget *a_parent = nullptr);
};

#endif // COMBOBOXLISTVIEW_H
