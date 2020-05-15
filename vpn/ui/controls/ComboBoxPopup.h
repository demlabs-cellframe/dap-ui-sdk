#ifndef COMBOBOXPOPUP_H
#define COMBOBOXPOPUP_H

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
class ComboBoxPopup;
}

class ComboBoxPopup : public CustomComboBoxPopup
{
    Q_OBJECT

public:
    explicit ComboBoxPopup(QMainWindow *a_parent = nullptr);

protected:
    virtual QString captionLabelName() override;
    virtual QString listViewName() override;

#ifndef ANDROID
    virtual void showEvent(QShowEvent *event) override;
    virtual void setVisible(bool a_visible);

    bool m_canBeHidden;
public slots:
    void allowClosingAndHide();

signals:
    void closingStarted();

#endif

private:
    const QString LVW_LIST_NAME    = "lvwList";
    const QString LBL_CAPTION_NAME = "lblCaption";
};

#endif // COMBOBOXPOPUP_H
