#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QComboBox>

#include "AdaptiveScreen.h"

#include "ui_MainScreen.h"
#include "CustomComboBox.h"

class MainScreen : public AdaptiveScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreen(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;
};

#endif // MAINSCREEN_H
