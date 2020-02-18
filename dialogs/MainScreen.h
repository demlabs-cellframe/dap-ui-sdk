#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QComboBox>
#include <QStateMachine>

#include "AdaptiveScreen.h"
#include "CustomComboBox.h"
#include "vpnDefine.h"

#include "ui_MainScreen.h"


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

public:

    void setState(ConnectionStates a_state);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    virtual void initVariantUi(QWidget *a_widget) override;


    const QString LBL_STATUS_MESSAGE = "lblStatusMessage";
    const QString BTN_SWITCH         = "btnSwitch";
    const QString CBB_SERVER         = "cbbServer";

signals:
    void connectionSwitched();

private:
    static QString statusText(ConnectionStates a_state);
};

#endif // MAINSCREEN_H
