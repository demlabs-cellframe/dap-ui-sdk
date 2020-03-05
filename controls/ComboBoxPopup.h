#ifndef COMBOBOXPOPUP_H
#define COMBOBOXPOPUP_H

#include <QWidget>
#include <QComboBox>

#include "AdaptiveWidget.h"
#include "CustomComboBoxPopup.h"
#include "WidgetDelegateListView.h"

namespace Ui {
class ComboBoxPopup;
}

class ComboBoxPopup : public CustomComboBoxPopup
{
    Q_OBJECT

public:
    explicit ComboBoxPopup(QWidget *a_parent = nullptr);

//    void setComboBox(QComboBox* a_combobox);

    template<typename T>
    void setDelegateFactory();

    void setCaption(const QString& a_caption);

protected:
    virtual QString listViewName() override;


private:
    const QString LVW_LIST_NAME    = "lvwList";
    const QString LBL_CAPTION_NAME = "lblCaption";
};


template<typename T>
void ComboBoxPopup::setDelegateFactory()
{
    T* delFactory = new T;
    for (WidgetDelegateListView* lstView: this->getTheSameWidgets<WidgetDelegateListView>(listViewName()))
        lstView->setDelegateFactory(delFactory);
}



#endif // COMBOBOXPOPUP_H
