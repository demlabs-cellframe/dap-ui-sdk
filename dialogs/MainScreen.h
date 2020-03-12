#ifndef MAINSCREEN_H
#define MAINSCREEN_H


#include <QStateMachine>

#include "AdaptiveScreen.h"

#include "vpnDefine.h"


#include "defines.h"
#include "DapDataLocal.h"
#include "StyledDropShadowEffect.h"

#ifdef Q_OS_ANDROID
#include "ui_MainScreenMobile.h"
#include <QFrame>

#else
#include "ui_MainScreen.h"

#include <QTimer>
#include "CustomComboBoxPopup.h"
#include "CustomComboBox.h"

#include <QComboBox>
#endif

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

#ifdef Q_OS_ANDROID
    const QString FRM_CONNECT       = "frmConnect";
    const QString FRM_INFO          = "frmInfo";
    const QString FRM_STATUS        = "frmStatus";
    const QString BTN_CHANGE_SERVER = "btnChangeServer";
    const QString LBL_ACTUAL_SERVER = "lblActualServer";
    const QString BTN_CONNECT       = "btnConnect";
    const QString LBL_LOGIN_TIME    = "lblLoginTime";
    const QString LBL_TIME_CONNECT  = "lblTimeConnect";
    const QString LBL_PACKETS_REC   = "lblPacketsRec";
    const QString LBL_PACKETS_SENT  = "lblPacetsSent";
    const QString CBB_AUTH          = "cbbAuth";
    const QString CBB_STREAM        = "cbbStream";
    const QString CBB_NETWORK       = "cbbNetwork";
#else
    const QString LBL_STATUS_MESSAGE = "lblStatusMessage";
    const QString BTN_SWITCH         = "btnSwitch";
    const QString CBB_SERVER         = "cbbServer";
#endif


signals:
    void connectionSwitched();
    void serverChanged(const QString& serverName);

private:
    static QString statusText(ConnectionStates a_state);
#ifdef Q_OS_ANDROID

#else
    QAbstractItemModel *m_serversModel = nullptr;
#endif
};

#endif // MAINSCREEN_H
