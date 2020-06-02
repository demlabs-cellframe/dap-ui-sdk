#ifndef SCREENCOMBOBOXPOPUP_H
#define SCREENCOMBOBOXPOPUP_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QListView>
#include <QMainWindow>

#include "AdaptiveWidget.h"
#include "CustomComboBoxPopup.h"
#include "WidgetDelegateListView.h"
#include "ComboBoxPopup.h"

namespace Ui {
class ComboBoxPopup;
}

class ScreenComboBoxPopup : public ComboBoxPopup
{
    Q_OBJECT

public:
    explicit ScreenComboBoxPopup(QWidget *a_parent = nullptr);

protected:
    virtual void showEvent(QShowEvent *event) override;
    virtual void setVisible(bool a_visible) override;

public slots:
    void allowClosingAndHide();

signals:
    void closingStarted();

private:
    const QString LVW_LIST_NAME    = "lvwList";
    const QString LBL_CAPTION_NAME = "lblCaption";

    bool m_canBeHidden;
};

#endif // SCREENCOMBOBOXPOPUP_H
