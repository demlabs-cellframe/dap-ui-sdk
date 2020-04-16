#ifndef SCREENDASHBOARD_H
#define SCREENDASHBOARD_H

#include <QGraphicsScene>
#include "DapUiScreen.h"
#include "schedules.h"
#include <QComboBox>
#include <QMap>
#include <QDateTime>
#include <QTimer>
#include <QtWidgets>
#include <QFrame>
#include "controls/ComboBoxDelegate.h"
#include "controls/ServersComboBox.h"

using TrafficSpeed = QPair<double, double>;

class ScreenDashboard : public DapUiScreen
{
    Q_OBJECT
    
    /// Timeout for total connection time calculator in milliseconds.
    const ushort TIMEOUT_CONNECT_CALCULATION{1000};
    /// Timeout for the received / sent data rate calculator in milliseconds.
    const ushort TIMEOUT_SPEED_CALCULATION{2000};
    
    QGraphicsScene *m_scene;
    int m_sceneWidth;
    int m_sceneHeight;
    Schedules schedules;
    /// Pointer to the drop-down list of available servers "Servers".
    ServersComboBox *cbUpstream {nullptr};
    /// Flag multirounted state.
    bool        m_multiroutingState{false};
    /// Graphics styles.
    DapGraphicSceneStyle    mGraphicSceneStyle;
    /// Timer calculator for data transmission/reception.
    QTimer  m_speedCalculationTimer;
    /// Connection time calculator timer.
    QTimer  m_timeConnectedTimer;
    /// Total connection time in seconds.
    uint64_t m_timeConnected{0};
    /// The amount of received/sent data at the current time.
    TrafficSpeed m_dataNew {0,0};
    /// The number of received / sent data for the previous cycle.
    TrafficSpeed m_dataOld {0,0};
    
protected:
    void initUi(QWidget * a_w,ScreenRotation a_rotation);
    /// Set flag of multiroute state.
    /// @param state Multirouout state.
    inline void setMultiroutingState(bool state);
    /// Get time interval connection.
    /// @details It has the format: days:hours:minutes:seconds.
    /// @param dateBegin Connection establishment timing.
    /// @param dateEnd Time of the current connection.
    /// @return Connection time interval.
    inline QString getUptime(quint64 seconds);
    /// Calculate data transmission/reception speed in Mbps.
    /// @details The timeout for calculating the speed is assumed to be 2 seconds.
    /// @return Speed of received/sent data.
    inline TrafficSpeed calculateSpeedMbps();
    
public:
    void authorzeIndicator(bool on);
    void streamIndicator(bool on);
    void tunnelIndicator(bool on);

    QGraphicsScene* getScene() {return m_scene;}
    int getSceneWidth() {return m_sceneWidth;}
    int getSceneHeight() {return m_sceneHeight;}
    
    /// Add styles to the graphic element.
    /// @param widget Graphic element.
    /// @param styleWidget Graphic styles.
    void addItemGraphicSceneStyle(const QString &widget, const QString &styleWidget);
    /// Delete styles for the graphic element.
    /// @param widget Graphic element.
    void removeItemGraphicSceneStyle(const QString &widget);

    ScreenDashboard(QObject * a_parent, QStackedWidget * a_sw);

    QWidget *btDisconnect;
public slots:
    void drawShedules(int readBytes, int writeBytes, int readPackage, int writePackage);
    /// Multirouout state.
    /// @return Сurrent multirouout state.
    bool isMultirouting();
    /// Stop calculators for calculating the speed of sent/received
    /// data and calculating the total connection time.
    void stopCalculationTimers();
    
private slots:
    /// Start calculation of connection data.
    /// @param startTime Last connection time.
    void startCalculateConnectionData(const QDateTime &startTime);
    
signals:
    void reconnectSignal(QString adr, QString ip);
    void currentUpstreamNameChanged(const QString &name);
    /// The signal is emitted in case of a change in the state of multi-routing.
    /// @param state New multirouout state.
    void multiroutingChanged(bool state);
};

#endif // DAPUIDASHBOARD_H
