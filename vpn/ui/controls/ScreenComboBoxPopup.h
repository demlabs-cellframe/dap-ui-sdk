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
    virtual void initVariantUi(QWidget * a_widget) override;

#ifndef ANDROID
protected:
    virtual void setVisible(bool a_visible) override;

public slots:
    void allowClosingAndHide();

signals:
    void closingStarted();

private:
    bool m_canBeHidden;
#endif
};

#endif // SCREENCOMBOBOXPOPUP_H
