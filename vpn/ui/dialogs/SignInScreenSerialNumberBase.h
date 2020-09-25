#pragma once

#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include <QStateMachine>
#include <QStandardItemModel>

#include "ScreenInfo.h"
#include "vpnDefine.h"
#include "defines.h"
#include "ui_SignInScreen.h"
#include "StyledDropShadowEffect.h"
#include "CustomLineEdit.h"
#include "PasswordLineEdit.h"
#include "ClickableLabel.h"
#include "CustomLicenseLineEdit.h"
#include "ScreenWithScreenPopupsAbstract.h"
#include "SerialNumberLineEdit.h"
#include "BrandTextProperties.h"

#ifdef Q_OS_ANDROID
#include "WidgetInputSizeController.h"
#endif

#define STATUS_TEXT_CONNECTING_TO_SERVICE "Connecting to service..."


/// from "BrandTextProperties.h"
#ifndef LOGIN_BUTTON_TEXT_CONNECTING
#define LOGIN_BUTTON_TEXT_CONNECTING "CONNECTING..."
#endif

/// from "BrandTextProperties.h"
#ifndef LOGIN_BUTTON_TEXT_DEFAULT
#define LOGIN_BUTTON_TEXT_DEFAULT "START"
#endif

class SignInScreenSerialNumberBase : public ScreenWithScreenPopupsAbstract
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    SignInScreenSerialNumberBase(QWidget * a_parent);



    virtual QString screenName() override;
    static const QString SCREEN_NAME;

    QString serialKey();

public slots:
    void setState(ConnectionState a_state);
    void setActivated(bool a_activated);

    void setCurrentServerName(const QString &a_serverName);

    void setErrorMessage(const QString &a_errorMsg = "");

    void setSerialKey(const QString &a_serial);


signals:

    void activated();
    void unactivated();
    void serialKeyChanged(const QString &);
    void serialKeyEdited(const QString &);
    void serialKeyError();
    void serviceDisconnected();
    void serviceConnected();
    void connectionRequested();
    void connectionError();
    void connected();
    void serversListCleared();

    void serverChanged(int serverName);

protected:

    virtual void initVariantUi(QWidget *a_widget) override;

    virtual QList<CustomPopup *> customPopups() override;

    QScopedPointer<Ui::SignInScreen> m_ui;


    void adjustStateMachine();

    QString translatedErrorMsg(QString a_errorMsg);
    bool serialKeyIsEntered();
    bool serviceIsConnected();
    bool isLoadingState();


    QStateMachine *m_inputStates;
    QState *m_stt_serialKey;
    QState *m_stt_serialKey_unactivated;
    QState *m_stt_serialKey_unactivated_empty;
    QState *m_stt_serialKey_unactivated_entered;
    QState *m_stt_serialKey_unactivated_wrong;
    QState *m_stt_serialKey_activated;
    QState *m_stt_serverState;
    QState *m_stt_serverState_disconnected;
    QState *m_stt_serverState_loading;
    QState *m_stt_serverState_loading_serverList;
    QState *m_stt_serverState_loading_connecting;
    QState *m_stt_serviceState;
    QState *m_stt_serviceState_connected;
    QState *m_stt_serviceState_connecting;

    QString m_email;
    QString m_password;

    QString m_serial;

#ifdef Q_OS_ANDROID
    WidgetInputSizeController *m_widgetSizeController = nullptr;
#endif
};



