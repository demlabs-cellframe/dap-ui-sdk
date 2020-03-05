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


    const QString LBL_STATUS_MESSAGE_NAME = "lblStatusMessage";
    const QString BTN_SWITCH_NAME         = "btnSwitch";
    const QString CBB_SERVER_NAME         = "cbbServer";

    const QString CHOOSE_SERVER_CAPTION   = "Choose server";

signals:
    void connectionSwitched();
    void serverChanged(const QString& serverName);

private:
    static QString statusText(ConnectionStates a_state);

    QAbstractItemModel *m_serversModel = nullptr;
};

#endif // MAINSCREEN_H
