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

#ifndef ANDROID
    #include "ScreenComboBox.h"
#endif

#ifdef ANDROID
    #include <QFrame>
#else
    #include "CustomComboBoxPopup.h"
    #include <QComboBox>
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

    enum class IndicatorsUnits {Bytes, Packets};

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

    const QString BYTES = "Bytes";
    const QString PACKETS = "Packets";

    static QString toTimeString(quint64 seconds);
    QString statusText();

    bool indicatorUnitsIsBytes() const;
#ifndef ANDROID
    void setIndicatorUnits(const IndicatorsUnits &a_indicatorUnits);

    QString receivedIndicatorTitle() const;
    QString sendIndicatorTitle() const;
#endif

    IndicatorsUnits m_indicatorUnits;
    int m_bytesSent       = 0;
    int m_bytesReceived   = 0;
    int m_packetsSent     = 0;
    int m_packetsReceived = 0;

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

    void setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent);

    void startConnectionTimer(const QDateTime &a_startTime);
    void stopConnectionTimer();

signals:
    void disconnectionRequested();
    void serverChangingRequested(const QString& serverName);

    void changeBytesPackets(int a_bytesRead, int a_bytesWrite, int a_packetsRead, int a_packetsWrite);
    void setIndicatorsUnit(bool a_bytes_not_packets);

};

#endif // MAINSCREEN_H
