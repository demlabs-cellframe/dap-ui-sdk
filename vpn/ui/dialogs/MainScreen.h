#ifndef MAINSCREEN_H
#define MAINSCREEN_H


#include <QStateMachine>

#include "AdaptiveScreen.h"

#include "vpnDefine.h"


#include "defines.h"
#include "DapDataLocal.h"
#include "StyledDropShadowEffect.h"

#include <QTimer>
#include "ui_MainScreen.h"
#ifdef ANDROID
    #include <QFrame>
#else
    #include "CustomComboBoxPopup.h"
    #include <QComboBox>
#endif

class MainScreen : public AdaptiveScreen
{
    Q_OBJECT

    /// Timeout for total connection time calculator in milliseconds.
    const ushort TIMEOUT_CONNECT_CALCULATION{1000};
    /// Timeout for the received / sent data rate calculator in milliseconds.
    const ushort TIMEOUT_SPEED_CALCULATION{2000};

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
    QString getUptime(quint64 seconds);
    double calculateSpeedMbps();

    const QString BTN_CONNECTION    = "btnConnection";

    const QString CHB_AUTHORIZED      = "chbAuthorized";
    const QString CHB_STREAM_OPENED   = "chbStreamOpened";
    const QString CHB_VIRTUAL_NETWORK = "chbVirtualNetwork";

    const QString LBL_RECEIVED      = "lblReceived";
#ifdef ANDROID
    const QString FRM_CONNECT       = "frmConnect";
    const QString FRM_INFO          = "frmInfo";
    const QString FRM_STATUS        = "frmStatus";
    const QString BTN_CHANGE_SERVER = "btnChangeServer";
    const QString LBL_ACTUAL_SERVER = "lblActualServer";
    const QString LBL_LOGINED_TIME  = "lblLoginedTime";
    const QString LBL_CONNECTED_TIME = "lblConnectedTime";
    const QString LBL_SENT  = "lblSent";
#else
    const QString LBL_STATUS_MESSAGE        = "lblStatusMessage";
    const QString CBB_SERVER                = "cbbServer";
    const QString BTN_BYTES                 = "btnBytes";
    const QString BTN_PACKETS               = "btnPackets";
    const QString LBL_RECREIVED_TITLE       = "lblReceivedTitle";
    const QString LBL_DOWNLOADING_SPEED     = "lblDownloadingSpeed";
    const QString LBL_CONNECTED_TIME        = "lblConnectedTime";
    const QString LBL_DOWNLOADED            = "lblDownloaded";
#endif

signals:
    void disconnect();
    void serverChanged(const QString& serverName);

    void changeBytesPackets(int a_bytesRead, int a_bytesWrite, int a_packetsRead, int a_packetsWrite);
    void setIndicatorsUnit(bool a_bytes_not_packets);

public slots:

    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);

#ifndef ANDROID
    void setDownloadSpeed(double a_downloadSpeed);
    void setDownload(double a_download);
#endif
    void setBytesPackets(int a_bytesRead, int a_bytesWrite, int a_packetsRead, int a_packetsWrite);
    void setTimeConnected(uint64_t a_timeConnected);

    void startCalculateConnectionData(const QDateTime &startTime);
    void stopCalculationTimers();

private:
    static QString statusText(ConnectionStates a_state);
    bool m_bytes_not_packets;

    /// Timer calculator for data transmission/reception.
    QTimer  m_speedCalculationTimer;
    /// Connection time calculator timer.
    QTimer  m_timeConnectedTimer;
    /// Total connection time in seconds.
    uint64_t m_timeConnected{0};
    /// The amount of received/sent data at the current time.
    QPair<int, int> m_dataNew = {0, 0};
    /// The number of received / sent data for the previous cycle.
    QPair<int, int>  m_dataOld = {0, 0};

};

#endif // MAINSCREEN_H
