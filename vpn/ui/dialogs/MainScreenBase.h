#ifndef MAINSCREENBASE_H
#define MAINSCREENBASE_H


#include <QStateMachine>

#include "AdaptiveScreen.h"

#include "vpnDefine.h"

#include <QGraphicsScene>
#include "defines.h"
#include "schedules.h"
#include "DapDataLocal.h"
#include "StyledDropShadowEffect.h"

#include <QDateTime>
#include <QTimer>
#include "ui_MainScreen.h"
#include "Utilz.h"
#include "AppStyleSheetHandler.h"
#include "UiScaling.h"
#include "ComboBox.h"
#include <QDebug>
#include "BrandTextProperties.h"

#ifndef ANDROID
    #include "ScreenComboBox.h"
#endif

#ifdef ANDROID
    #include <QFrame>
#else
    #include "CustomComboBoxPopup.h"
    #include <QComboBox>
#endif


/// from "BrandTextProperties.h"
#ifndef DASHBOARD_TEXT_CONNECT
#define DASHBOARD_TEXT_CONNECT "CONNECT"
#endif

/// from "BrandTextProperties.h"
#ifndef DASHBOARD_TEXT_DISCONNECT
#define DASHBOARD_TEXT_DISCONNECT "DISCONNECT"
#endif


class MainScreenBase : public AdaptiveScreen
{
    Q_OBJECT

public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    /// @param a_sw Application window stack.
    MainScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setState(ConnectionState a_state);

    enum class IndicatorsUnits {Download, Upload};

    IndicatorsUnits indicatorUnits() const;


#ifndef Q_OS_ANDROID
    /// Add styles to the graphic element.
    /// @param widget Graphic element.
    /// @param styleWidget Graphic styles.
    void addItemGraphicSceneStyle(const QString &widget, const QString &styleWidget);
    /// Delete styles for the graphic element.
    /// @param widget Graphic element.
    void removeItemGraphicSceneStyle(const QString &widget);
#endif
protected:
    /// Timeout for total connection time calculator in milliseconds.
    const ushort CONNECTED_TIME_INTERVAL{1000};
    QScopedPointer<Ui::MainScreen> m_ui;
    QGraphicsScene *m_scene;
    Schedules schedules;

    /// Graphics styles.
    DapGraphicSceneStyle    mGraphicSceneStyle;
    QGraphicsScene* getScene() {return m_scene;}
    bool isUsingGraphic = false;

#ifndef Q_OS_ANDROID
    void initServerList();
#endif

    /// Form initialization.
    /// @param a_w Window GUI widget.
    /// @param a_rotation Device display orientation.
    void initVariantUi(QWidget *a_widget) override;

    void initGraphic(QGraphicsView *a_graphic);

    void updateSentRecievedIndicators();
    void updateTimeIndicators();

    uint64_t connectedTime();

    QDateTime loginTime() const;
    void setLoginTime(const QDateTime &loginTime);

    void setGraphicsHeight(int a_height);
    void setGraphicsWidth(int a_width);

    const QString EMPTY_TYME          = "...";

    const QString DOWNLOAD = "Download";
    const QString UPLOAD = "Upload";
    const QString RECEIVED = "received";
    const QString SENT = "sent";

//    const QString TEXT_CONNECT = tr("CONNECT");
//    const QString TEXT_DISCONNECT = tr("DISCONNECT");

    static QString toTimeString(quint64 seconds);
    QString statusText();

    bool indicatorUnitsIsDownload() const;
#ifndef ANDROID
    void setIndicatorUnits(const IndicatorsUnits &a_indicatorUnits);

    QString speedIndicatorTitle() const;
    QString bytesIndicatorTitle() const;
    QString packetsIndicatorTitle() const;
#endif

    IndicatorsUnits m_indicatorUnits;
    QString m_uploadSpeed       = "0 bps";
    QString m_downloadSpeed     = "0 bps";
    quint64 m_bytesSent       = 0;
    quint64 m_bytesReceived   = 0;
    quint64 m_packetsSent     = 0;
    quint64 m_packetsReceived = 0;

    QAbstractItemModel *m_serversModel = nullptr;

    /// Connection time calculator timer.
    QTimer  m_connectedTimer;
    /// Total connection time in seconds.
    QDateTime m_loginTime {};

    ConnectionState m_state {ConnectionState::Disconnected};
    QString m_currentServer {};


public slots:

    void setCurrentServer(const QString& a_currentServer);

    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);

    void setSentReceivedIndicators(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent);

    QString convertBytePerSecond(const quint64 &bytes_new);

    void startConnectionTimer(const QDateTime &a_startTime);
    void stopConnectionTimer();

signals:
    void disconnectionRequested();
    void serverChangingRequested(const QString& serverName);

    void changeBytesPackets(int a_bytesRead, int a_bytesWrite, int a_packetsRead, int a_packetsWrite);
    void setIndicatorsUnit(bool a_bytes_not_packets);

};

#endif // MAINSCREEN_H
