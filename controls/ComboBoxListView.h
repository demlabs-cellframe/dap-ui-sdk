#ifndef COMBOBOXLISTVIEW_H
#define COMBOBOXLISTVIEW_H

#ifdef ANDROID
    #include "CustomComboBoxListView.h"
    typedef CustomComboBoxListView ParentClass;
#else
    #include "WidgetDelegateListView.h"
    typedef WidgetDelegateListView ParentClass;
#endif



class ComboBoxListView: public ParentClass
{
public:
    ComboBoxListView(QWidget *a_parent = nullptr);

protected:

#ifndef ANDROID
    virtual WidgetDelegateBase* createWidgetDelegate() override;
#endif

};

#endif // COMBOBOXLISTVIEW_H
