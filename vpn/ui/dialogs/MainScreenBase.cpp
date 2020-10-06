#include "MainScreenBase.h"

const QString MainScreenBase::SCREEN_NAME = "Main";

MainScreenBase::MainScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());

    this->create(m_ui);

    AdaptiveScreen::initScreen(this);

    m_connectedTimer.setInterval(CONNECTED_TIME_INTERVAL);

    // Signal-slot connection that updates the total connection time with a timeout of 1 second
    connect(&m_connectedTimer, &QTimer::timeout, [this]
    {
        this->updateTimeIndicators();
    });

    connect(m_ui->btnConnection,&QPushButton::clicked,this,&MainScreenBase::disconnectionRequested);

#ifdef Q_OS_ANDROID

    //========================================================================
    m_ui->btnChangeServer ->setGraphicsEffect(new StyledDropShadowEffect(m_ui->btnChangeServer));
    m_ui->frmConnect      ->setGraphicsEffect(new StyledDropShadowEffect(m_ui->frmConnect)     );
    m_ui->frmInfo         ->setGraphicsEffect(new StyledDropShadowEffect(m_ui->frmInfo)        );
    m_ui->frmStatus       ->setGraphicsEffect(new StyledDropShadowEffect(m_ui->frmStatus)      );

    m_ui->btnChangeServer->hide();
#endif
}

QString MainScreenBase::screenName()
{
    return MainScreenBase::SCREEN_NAME;
}

void MainScreenBase::setState(ConnectionState a_state)
{
    if (a_state == m_state)
        return;
    m_state = a_state;

#ifndef Q_OS_ANDROID
    if(a_state == ConnectionState::Disconnected)
    {
        m_ui->btnConnection->setText(tr(DASHBOARD_TEXT_CONNECT));
    }
    else {
        m_ui->btnConnection->setText(tr(DASHBOARD_TEXT_DISCONNECT));
    }
#endif

    m_ui->lblStatusMessage->setText(this->statusText());
    this->setEnabled(a_state == ConnectionState::Connected);

    if(a_state == ConnectionState::Disconnected)
        this->stopConnectionTimer();
}

void MainScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}
#ifndef Q_OS_ANDROID
void MainScreenBase::initServerList()
{
    if (!m_serversModel)
    {
        m_serversModel = DapDataLocal::serversData();
    }
    m_ui->cbbServer->setModel(m_serversModel);
}
#endif
void MainScreenBase::setAuthorized(bool a_authorized /*= true*/)
{
    m_ui->chbAuthorized->setChecked(a_authorized);
}

void MainScreenBase::setStreamOpened(bool a_streamOpened /*= true*/)
{
    m_ui->chbStreamOpened->setChecked(a_streamOpened);
}

void MainScreenBase::setVirtualNetwork(bool a_virtualNetwork /*= true*/)
{
    m_ui->chbVirtualNetwork->setChecked(a_virtualNetwork);
}

void MainScreenBase::setSentReceivedIndicators(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent)
{
    m_uploadSpeed     = convertBytePerSecond(a_bytesSent - m_bytesSent);
    m_downloadSpeed   = convertBytePerSecond(a_bytesReceived - m_bytesReceived);
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;

    this->updateSentRecievedIndicators();

    if(this->isUsingGraphic)
    {
        schedules.addInp(m_bytesReceived);
        schedules.addOut(m_bytesSent);

        schedules.draw_chart(getScene());
        getScene()->update();
    }
}

QString MainScreenBase::convertBytePerSecond(const quint64 &byte)
{
    if (byte >= pow(2,20))
        return QString("%1 %2").arg(QString::number(byte/pow(2,20), 'f', 2)).arg("Mbps");
    else
        return QString("%1 %2").arg(QString::number(byte/pow(2,10), 'f', 2)).arg("Kbps");
}

uint64_t MainScreenBase::connectedTime()
{
    return m_loginTime.secsTo(QDateTime::currentDateTime());
}

void MainScreenBase::updateSentRecievedIndicators()
{
    //TODO:
    // Check why sigReadWriteBytesStat signal send wrong datas (bytes/packets are contrarily)
#if !defined Q_OS_ANDROID
    m_ui->lblSpeed->setText(this->indicatorUnitsIsDownload() ? m_downloadSpeed : m_uploadSpeed);
    m_ui->lblBytes->setText(this->indicatorUnitsIsDownload() ? Utils::convertByte(m_bytesReceived) : Utils::convertByte(m_bytesSent));
    m_ui->lblPackets->setText(this->indicatorUnitsIsDownload() ? QString::number(m_packetsReceived) : QString::number(m_packetsSent));
#endif
}

void MainScreenBase::updateTimeIndicators()
{
    QString loginTime;
    if (m_loginTime == QDateTime())
        loginTime = EMPTY_TYME;
    else if (m_loginTime.date() == QDate::currentDate())
        loginTime = m_loginTime.toString("hh:mm");
    else
        loginTime = m_loginTime.toString("MM-dd-yy hh:mm");
#if defined Q_OS_ANDROID
    m_ui->lblLoginTime->setText(loginTime);
#endif
    QString connectedTime;
    if (this->connectedTime() == 0)
        connectedTime = EMPTY_TYME;
    else
        connectedTime = MainScreenBase::toTimeString(this->connectedTime());

    m_ui->lblConnectedTime->setText(connectedTime);
}

QString MainScreenBase::statusText()
{
    switch (m_state)
    {
    case ConnectionState::Disconnected:
        return tr("Disconnected");
    case ConnectionState::Connecting:
        return tr("Connecting...");
    case ConnectionState::Connected:
        return tr("Connected to %1").arg(m_currentServer);
    case ConnectionState::Disconnecting:
        return tr("Disconnecting...");
    case ConnectionState::ServerChanging:
        return tr("Changing server...");
    default:
        return QString();
    }
}


bool MainScreenBase::indicatorUnitsIsDownload() const
{
#ifdef ANDROID
    return true;
}
#else
    return (this->indicatorUnits() == IndicatorsUnits::Download);
}

void MainScreenBase::setIndicatorUnits(const IndicatorsUnits &a_indicatorUnits)
{
    if (m_indicatorUnits == a_indicatorUnits)
        return;
    m_indicatorUnits = a_indicatorUnits;

    m_ui->btnDownload->setChecked(indicatorUnitsIsDownload());
    m_ui->btnDownload->setEnabled(!this->indicatorUnitsIsDownload());

    m_ui->btnUpload->setChecked(!this->indicatorUnitsIsDownload());
    m_ui->btnUpload->setEnabled( this->indicatorUnitsIsDownload());

    m_ui->lblSpeedTitle->setText(this->speedIndicatorTitle());
    m_ui->lblBytesTitle->setText(this->bytesIndicatorTitle());
    m_ui->lblPacketsTitle->setText(this->packetsIndicatorTitle());

    this->updateSentRecievedIndicators();
}

QString MainScreenBase::speedIndicatorTitle() const
{
    return (this->indicatorUnitsIsDownload() ? DOWNLOAD : UPLOAD) + " speed";
}

QString MainScreenBase::bytesIndicatorTitle() const
{
    return "Bytes " + (this->indicatorUnitsIsDownload() ? RECEIVED : SENT);
}

QString MainScreenBase::packetsIndicatorTitle() const
{
    return "Packets " + (this->indicatorUnitsIsDownload() ? RECEIVED : SENT);
}
#endif

QDateTime MainScreenBase::loginTime() const
{
    return m_loginTime;
}

void MainScreenBase::setLoginTime(const QDateTime &loginTime)
{
    m_loginTime = loginTime;

    this->updateTimeIndicators();
}

MainScreenBase::IndicatorsUnits MainScreenBase::indicatorUnits() const
{
    return m_indicatorUnits;
}

void MainScreenBase::setCurrentServer(const QString &a_currentServer)
{
    qDebug() << "MainScreen::setCurrentServer:" << a_currentServer;
    if (m_currentServer == a_currentServer)
        return;
    m_currentServer = a_currentServer;

    m_ui->lblStatusMessage->setText(this->statusText());
}


QString MainScreenBase::toTimeString(quint64 seconds)
{
    const qint64 DAY = 86400;
    qint64 days = seconds / DAY;
    QTime t = QTime(0,0).addSecs(seconds % DAY);

    QString res = QString();
    if (days > 0)
        res.sprintf("%d %02d:%02d:%02d", static_cast<int>(days), t.hour(), t.minute(), t.second());
    else
        res.sprintf("%02d:%02d:%02d", t.hour(), t.minute(), t.second());

    return res;
}

void MainScreenBase::startConnectionTimer(const QDateTime &a_startTime)
{   
    this->setLoginTime(a_startTime);

    m_connectedTimer.start();
}

void MainScreenBase::stopConnectionTimer()
{
    this->setLoginTime(QDateTime());

    m_connectedTimer.stop();
}

#ifndef Q_OS_ANDROID

void MainScreenBase::initGraphic(QGraphicsView *a_graphic)
{
    this->isUsingGraphic = true;
    m_scene = new QGraphicsScene();

    a_graphic->setScene(m_scene);

    connect(m_ui->cbbServer, SIGNAL(activated(const QString &))  , this, SIGNAL(serverChangingRequested(const QString &)));

    connect(m_ui->btnDownload,&QPushButton::clicked,[=]{
        setIndicatorUnits(IndicatorsUnits::Download);
    });
    connect(m_ui->btnUpload,&QPushButton::clicked,[=]{
        setIndicatorUnits(IndicatorsUnits::Upload);
    });
    // Set styles for graphics.
    addItemGraphicSceneStyle("shChartDownload",   AppStyleSheetHandler::getWidgetStyleSheet("#shChartDownload", "active"));
    addItemGraphicSceneStyle("shChartUpload",     AppStyleSheetHandler::getWidgetStyleSheet("#shChartUpload"  , "active"));
    addItemGraphicSceneStyle("shGrid",            AppStyleSheetHandler::getWidgetStyleSheet("#shGrid"         , "active"));
    addItemGraphicSceneStyle("graphicsLines",     AppStyleSheetHandler::getWidgetStyleSheet("#graphicsLines"  , "active"));

}
/// Add styles to the graphic element.
/// @param widget Graphic element.
/// @param styleWidget Graphic styles.
void MainScreenBase::addItemGraphicSceneStyle(const QString &widget, const QString &styleWidget)
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
void MainScreenBase::removeItemGraphicSceneStyle(const QString &widget)
{
    mGraphicSceneStyle.remove(widget);
    schedules.setStyle(mGraphicSceneStyle);
}
#endif
