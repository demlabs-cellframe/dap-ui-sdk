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

public slots:
    void allowClosingAndHide();
    virtual void setVisible(bool a_visible) override;

signals:
    void closingStarted();

protected:
    virtual void initVariantUi(QWidget * a_widget) override;

private:
    bool m_canBeHidden;
};

#endif // SCREENCOMBOBOXPOPUP_H
