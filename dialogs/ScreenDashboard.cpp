#include <QtDebug>


#ifdef DAP_PLATFORM_DESKTOP
#include "ui_dashboard_desktop.h"
#include "ui_dashboard_desktop_big.h"
#include "ui_dashboard_desktop_small.h"
#else
#include "ui_dashboard_mobile_hor.h"
#include "ui_dashboard_mobile_hor_big.h"
#include "ui_dashboard_mobile_hor_small.h"
#include "ui_dashboard_mobile_ver.h"
#include "ui_dashboard_mobile_ver_big.h"
#include "ui_dashboard_mobile_ver_small.h"
#endif

#include <QCheckBox>
#include <QDateTime>
#include "ScreenDashboard.h"
#include "DapDataLocal.h"
#include "AppStyleSheetHandler.h"
#include "StyledDropShadowEffect.h"

Q_DECLARE_METATYPE(DapServerInfo);

/**
 * @brief DapUiScreenDashboard::initUiCommonAfter
 * @param a_w
 */
void ScreenDashboard::initUi(QWidget * a_w,ScreenRotation a_rotation)
{
    Q_UNUSED(a_rotation);

    QMap<QString, QString> serverKeys;
    QMap<QString, QString> countryFlags;
    
    // создаём сцену
    m_scene = new QGraphicsScene();
    QGraphicsView *graphicsView = a_w->findChild<QGraphicsView*>("graphicsView");
    graphicsView->setScene(m_scene);

    QWidget *wtMultirouting = a_w->findChild<QWidget*>("wtMultirouting");
    if (wtMultirouting)
        wtMultirouting->setGraphicsEffect(new StyledDropShadowEffect(wtMultirouting));


#ifndef Q_OS_ANDROID
    CustomPlacementButton *btMultirouting = a_w->findChild<CustomPlacementButton*>("btMultirouting");
    Q_ASSERT(btMultirouting);
    btMultirouting->addSubcontrol("checkbox");
#endif

    cbUpstream = a_w->findChild<ServersComboBox*>();
#ifndef Q_OS_ANDROID
    Q_ASSERT(cbUpstream);
#endif

    StyleSheatSearchPar searchPar;
    searchPar.widgetName =  "#" + cbUpstream->objectName();
    searchPar.subcontrol = "down-arrow";

    QString arrowStyleSheet = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);

    searchPar.pseudoClass = "on";
    QString arrowStyleSheetOn = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);

    searchPar.pseudoClass = "hover";
    QString arrowStyleSheetHover = AppStyleSheetHandler::getWidgetStyleSheet(searchPar);

    cbUpstream->setArrowStyles(arrowStyleSheet, arrowStyleSheetOn, arrowStyleSheetHover);

    int sepIndex = 0;
    for(const DapServerInfo& i: DapDataLocal::me()->servers()) {
        cbUpstream->addItem(QIcon(DapDataLocal::me()->locationToIconPath(i.location)),
                            i.name , QVariant::fromValue(i));
        if (sepIndex != 0)
            cbUpstream->insertSeparator(sepIndex*2-1);
        sepIndex++;
    }

    DapDataLocal::me()->connectComboBox(cbUpstream);
    cbUpstream->setCurrentText( DapDataLocal::me()->serverName() );

    connect(cbUpstream, SIGNAL(currentTextChanged(QString)), this, SIGNAL(currentUpstreamNameChanged(QString)));

#ifndef Q_OS_ANDROID

    QWidget *btDisconnect = a_w->findChild<CustomPlacementButton*>("btDisconnect");
    if (btDisconnect)
        btDisconnect->setGraphicsEffect(new StyledDropShadowEffect(wtMultirouting));
#endif


    QRect rect = graphicsView->geometry();
    m_sceneHeight = rect.height();
    m_sceneWidth  = rect.width();

    graphicsView->setSceneRect(0,0,m_sceneWidth-3, m_sceneHeight-3);

    //***********************
    // Hide btMultirouting and Line under it. Remove this code for show btMultirouting
#ifndef DAP_PLATFORM_MOBILE
    btMultirouting->hide();
    a_w->findChild<QFrame*>("lineMultirouting")->hide();
#endif
}

void ScreenDashboard::authorzeIndicator(bool on)
{
    invokeMethods("lbStream_on", "setVisible", Q_ARG(bool, on));
    invokeMethods("lbStream_off", "setVisible", Q_ARG(bool, !on));
}

void ScreenDashboard::streamIndicator(bool on)
{
    invokeMethods("lbIpAddr_on", "setVisible", Q_ARG(bool, on));
    invokeMethods("lbIpAddr_off", "setVisible", Q_ARG(bool, !on));
}

void ScreenDashboard::tunnelIndicator(bool on)
{
    invokeMethods("lbInterface_on", "setVisible", Q_ARG(bool, on));
    invokeMethods("lbInterface_off", "setVisible", Q_ARG(bool, !on));
}

void ScreenDashboard::drawShedules(int readBytes, int writeBytes, int readPackage, int writePackage) {
    schedules.addInp(readBytes);
    schedules.addOut(writeBytes);
    
    m_dataNew = TrafficSpeed(readBytes, writeBytes);
    
    setVars("lbReceived","text",tr("%1KB").arg(readBytes));
    setVars("lbSent","text",tr("%1KB").arg(writeBytes));
    setVars("lblPacketsRead","text",tr("%1").arg(readPackage));
    setVars("lblPacketsWritten","text",tr("%1").arg(writePackage));

    schedules.draw_chart(getScene(),
                         getSceneWidth() - 3,
                         getSceneHeight() - 3);
    getScene()->update();
}

/// Set flag of multiroute state.
/// @param state Multirouout state.
void ScreenDashboard::setMultiroutingState(bool state)
{
    m_multiroutingState = state;
    emit multiroutingChanged(m_multiroutingState);
}

/// Multirouout state.
/// @return Сurrent multirouout state.
bool ScreenDashboard::isMultirouting()
{
    return m_multiroutingState;
}

/**
 * @brief DapUiScreenDashboard::DapUiScreenDashboard
 * @param a_parent
 * @param a_sw
 */
ScreenDashboard::ScreenDashboard(QObject * a_parent, QStackedWidget * a_sw)
    :DapUiScreen(a_parent,a_sw)
{
    m_speedCalculationTimer.setInterval(TIMEOUT_SPEED_CALCULATION);
    m_timeConnectedTimer.setInterval(TIMEOUT_CONNECT_CALCULATION);

    // Signal-slot connection that updates the total connection time with a timeout of 1 second
    connect(&m_timeConnectedTimer, &QTimer::timeout, [=]
    {
        ++m_timeConnected;
        setVars("lblTimeConnected","text",tr("%1").arg(getUptime(m_timeConnected)));
    });
    // Signal-slot connection, running a calculator for calculating the speed 
    // of receiving / transmitting data on a timeout of 2 seconds.
    connect(&m_speedCalculationTimer, &QTimer::timeout, [=]
    {
        TrafficSpeed speed = calculateSpeedMbps();
        setVars("lblDownloadSpeed","text",tr("%1Mbps").arg(QString().number(speed.first, 'f', 3)));
        setVars("lblUploadSpeed","text",tr("%1Mbps").arg(QString().number(speed.second, 'f', 3)));
    });

#ifdef DAP_PLATFORM_MOBILE
//    create<Ui::DashboardHor,Ui::DashboardHorSmall,Ui::DashboardHorBig,
//            Ui::DashboardVer,Ui::DashboardVerSmall,Ui::DashboardVerBig>();
    create<Ui::DashboardHorSmall,Ui::DashboardHorSmall,Ui::DashboardHorSmall,Ui::DashboardHorSmall,Ui::DashboardHorSmall,Ui::DashboardHorSmall>();																   
#else
    create<Ui::Dashboard,Ui::DashboardSmall,Ui::DashboardBig>();
#endif
}

/// Add styles to the graphic element.
/// @param widget Graphic element.
/// @param styleWidget Graphic styles.
void ScreenDashboard::addItemGraphicSceneStyle(const QString &widget, const QString &styleWidget)
{
    QStringList strParametrs = styleWidget.split(";");
    strParametrs.removeAll("");
    QMap<QString, QVariant> param;
    for(QString str : strParametrs)
    {
        QStringList tempParam = str.split(":");
        param.insert(tempParam.at(0), tempParam.at(1));
    }
    mGraphicSceneStyle.insert(widget, param);
    
    schedules.setStyle(mGraphicSceneStyle);
}

/// Delete styles for the graphic element.
/// @param widget Graphic element.
void ScreenDashboard::removeItemGraphicSceneStyle(const QString &widget)
{
    mGraphicSceneStyle.remove(widget);
    schedules.setStyle(mGraphicSceneStyle);
}

/// Get time interval connection.
/// @details It has the format: days:hours:minutes:seconds.
/// @param dateBegin Connection establishment timing.
/// @param dateEnd Time of the current connection.
/// @return Connection time interval.
QString ScreenDashboard::getUptime(quint64 seconds)
{
    const qint64 DAY = 86400;
    qint64 days = seconds / DAY;
    QTime t = QTime(0,0).addSecs(seconds % DAY);
    QString res;
    res.sprintf("%d d %02d:%02d:%02d", days, t.hour(), t.minute(), t.second());
    return res;
}

/// Calculate data transmission/reception speed in Mbps.
/// @details The timeout for calculating the speed is assumed to be 2 seconds.
/// @return Speed of received/sent data.
TrafficSpeed ScreenDashboard::calculateSpeedMbps()
{
    // Translation milliseconds to seconds
    ushort timeout = TIMEOUT_SPEED_CALCULATION/1000;
    
    TrafficSpeed speedNew {0,0};
    speedNew.first = 0.008*(m_dataNew.first - m_dataOld.first)/timeout;
    speedNew.second = 0.008*(m_dataNew.second - m_dataOld.second)/timeout;
    m_dataOld = m_dataNew;
    
    return speedNew;
}

/// Start calculation of connection data.
/// @param startTime Last connection time.
void ScreenDashboard::startCalculateConnectionData(const QDateTime &startTime)
{
    setVars("lblLastConnection","text",tr("%1").arg(startTime.toString("MM-dd-yy hh:mm")));
    m_timeConnected = startTime.secsTo(QDateTime::currentDateTime());
    setVars("lblTimeConnected","text",tr("%1").arg(getUptime(m_timeConnected)));
    m_speedCalculationTimer.start();
    m_timeConnectedTimer.start();
}

/// Stop calculators for calculating the speed of sent/received
/// data and calculating the total connection time.
void ScreenDashboard::stopCalculationTimers()
{
    m_timeConnectedTimer.stop();
    m_speedCalculationTimer.stop();
    m_dataNew = m_dataOld = {0,0};
}
