#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <QComboBox>
#include "AdaptiveScreen.h"
#include "DapUiScreen.h"

#include "ui_SettingsScreen.h"
#include "StyledDropShadowEffect.h"
#include "CustomComboBox.h"
#include "CustomPlacementButton.h"
#include "ComboBox.h"

class SettingsScreen : public AdaptiveScreen
{
    Q_OBJECT

public:
    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    SettingsScreen(QWidget * a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;
protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;

    const QString CBB_LANGUAGE      = "cbbLanguage";

#ifdef Q_OS_ANDROID
    const QString BTN_LOG_OUT       = "btnLogOut";
    const QString BTN_RENEW         = "btnRenew";
#else
    const QString CBB_BALABCE       = "cbbBalance";
    const QString CBB_BANDWIDTH     = "cbbBandwidth";
    const QString CBB_ENCRYPTION    = "cbbEncryption";
    const QString CBB_UPTIME        = "cbbUptime";
#endif
};
#endif // SETTINGSSCREEN_H
