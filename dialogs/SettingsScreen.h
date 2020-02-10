#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <QComboBox>

#include "DapUiScreen.h"

#include "ui_SettingsScreen.h"
#include "StyledDropShadowEffect.h"
#include "CustomComboBox.h"

class SettingsScreen : public DapUiScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    SettingsScreen(QObject * a_parent, QStackedWidget * a_sw);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initUi(QWidget * a_w, ScreenRotation a_rotation) override;
};
#endif // SETTINGSSCREEN_H
