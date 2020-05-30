#ifndef SERVERSCOMBOBOXPOPUP_H
#define SERVERSCOMBOBOXPOPUP_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QListView>
#include <QMainWindow>

#include "AdaptiveWidget.h"
#include "CustomComboBoxPopup.h"
#include "WidgetDelegateListView.h"

namespace Ui {
class ServersComboBoxPopup;
}

class ServersComboBoxPopup : public CustomComboBoxPopup
{
    Q_OBJECT

public:
    explicit ServersComboBoxPopup(QMainWindow *a_parent = nullptr);

protected:
    virtual QString captionLabelName() override;
    virtual QString listViewName() override;

private:
    const QString LVW_LIST_NAME    = "lvwList";
    const QString LBL_CAPTION_NAME = "lblCaption";


};

#endif // SERVERSCOMBOBOXPOPUP_H

