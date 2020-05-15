#include "MainScreen.h"
#include "Utilz.h"

#include <QTime>

const QString MainScreen::SCREEN_NAME = "Main";

MainScreen::MainScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    this->setObjectName(this->screenName());
    create<Ui::MainScreen>();

    AdaptiveScreen::initScreen(this);

    m_speedCalculationTimer.setInterval(TIMEOUT_SPEED_CALCULATION);
    m_timeConnectedTimer.setInterval(TIMEOUT_CONNECT_CALCULATION);

    // Signal-slot connection that updates the total connection time with a timeout of 1 second
    connect(&m_timeConnectedTimer, &QTimer::timeout, [=]
    {
        ++m_timeConnected;
        setTimeConnected(m_timeConnected);
    });

#ifndef Q_OS_ANDROID
    // Signal-slot connection, running a calculator for calculating the speed
    // of receiving / transmitting data on a timeout of 2 seconds.
    connect(&m_speedCalculationTimer, &QTimer::timeout, [=]
    {
        setDownloadSpeed(calculateSpeedMbps());
    });
#endif

}

QString MainScreen::screenName()
{
    return MainScreen::SCREEN_NAME;
}

void MainScreen::setState(ConnectionStates a_state)
{
#ifdef Q_OS_ANDROID
    Q_UNUSED(a_state)
#else
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::TEXT , statusText(a_state));
    this->setChildProperties(LBL_STATUS_MESSAGE, Properties::STATE, a_state);

    this->updateChildStyle  (LBL_STATUS_MESSAGE);
    if(a_state == ConnectionStates::Disconnected)
        this->stopCalculationTimers();
#endif
}

void MainScreen::initVariantUi(QWidget *a_widget)
{
    this->connectBtnToSignall(BTN_CONNECTION, &MainScreen::disconnect, a_widget);

    QCheckBox *chbAuthorized    ; Utils::findChild(a_widget, CHB_AUTHORIZED     , chbAuthorized);
    QCheckBox *chbStreamOpened  ; Utils::findChild(a_widget, CHB_STREAM_OPENED  , chbStreamOpened);
    QCheckBox *chbVirtualNetwork; Utils::findChild(a_widget, CHB_VIRTUAL_NETWORK, chbVirtualNetwork);


#ifdef Q_OS_ANDROID

    QFrame *frmConnect          = a_widget->findChild<QFrame        *>(FRM_CONNECT);        Q_ASSERT(frmConnect);
    QFrame *frmInfo             = a_widget->findChild<QFrame        *>(FRM_INFO);           Q_ASSERT(frmInfo);
    QFrame *frmStatus           = a_widget->findChild<QFrame        *>(FRM_STATUS);         Q_ASSERT(frmStatus);
    QPushButton *btnChangeServer = a_widget->findChild<QPushButton  *>(BTN_CHANGE_SERVER);  Q_ASSERT(btnChangeServer);
    QLabel *lblActualServer     = a_widget->findChild<QLabel        *>(LBL_ACTUAL_SERVER);  Q_ASSERT(lblActualServer);

    QLabel *lblLoginTime        = a_widget->findChild<QLabel        *>(LBL_LOGINED_TIME);     Q_ASSERT(lblLoginTime);
    QLabel *lblTimeConnect      = a_widget->findChild<QLabel        *>(LBL_CONNECTED_TIME);   Q_ASSERT(lblTimeConnect);
    QLabel *lblPacketsRec       = a_widget->findChild<QLabel        *>(LBL_RECEIVED);    Q_ASSERT(lblPacketsRec);
    QLabel *lblSent             = a_widget->findChild<QLabel        *>(LBL_SENT);   Q_ASSERT(lblSent);



    //To test appearance
    chbAuthorized->setChecked(true);
    chbStreamOpened->setChecked(true);
    chbVirtualNetwork->setChecked(true);


    //========================================================================
    btnChangeServer->setGraphicsEffect(new StyledDropShadowEffect(btnChangeServer));
    frmConnect->setGraphicsEffect(new StyledDropShadowEffect(frmConnect));
    frmInfo->setGraphicsEffect(new StyledDropShadowEffect(frmInfo));
    frmStatus->setGraphicsEffect(new StyledDropShadowEffect(frmStatus));

    btnChangeServer->hide();

#else
//    QComboBox *cbbServer            = a_widget->findChild<QComboBox*>(CBB_SERVER);          Q_ASSERT(cbbServer  );
    QPushButton *btnBytes           = a_widget->findChild<QPushButton*>(BTN_BYTES);         Q_ASSERT(btnBytes  );
    QPushButton *btnPackets         = a_widget->findChild<QPushButton*>(BTN_PACKETS);       Q_ASSERT(btnPackets  );

    QLabel *lblReceivedTitle   ; Utils::findChild(a_widget, LBL_RECREIVED_TITLE  , lblReceivedTitle   );
//    QLabel *lblDownloadingSpeed; Utils::findChild(a_widget, LBL_DOWNLOADING_SPEED, lblDownloadingSpeed);
//    QLabel *lblConnectedTime   ; Utils::findChild(a_widget, LBL_DOWNLOADING_SPEED, lblConnectedTime);

//    QLabel *lblDownload             = a_widget->findChild<QLabel*>(LBL_DOWNLOADED)    ;     Q_ASSERT(lblDownloaded  );
//    QLabel *lblBytesPackets         = a_widget->findChild<QLabel*>(LBL_PATES_PACKETS) ;     Q_ASSERT(lblBytesPackets  );
//    QLabel *lblStatusMessage        = a_widget->findChild<QLabel*>(LBL_STATUS_MESSAGE);     Q_ASSERT(lblStatusMessage  );


    connect(btnBytes,&QPushButton::clicked,[=]{
        btnBytes->setChecked(true);
        btnPackets->setChecked(false);
        lblReceivedTitle->setText("Bytes Received");
        m_bytes_not_packets = true;
    });
    connect(btnPackets,&QPushButton::clicked,[=]{
        btnBytes->setChecked(false);
        btnPackets->setChecked(true);
        lblReceivedTitle->setText("Packets Received");
        m_bytes_not_packets = false;
    });

#endif

}

void MainScreen::setAuthorized(bool a_authorized /*= true*/)
{
    this->setChildProperties(CHB_AUTHORIZED, Properties::CHECKED, a_authorized);
}

void MainScreen::setStreamOpened(bool a_streamOpened /*= true*/)
{
    this->setChildProperties(CHB_STREAM_OPENED, Properties::CHECKED, a_streamOpened);
}

void MainScreen::setVirtualNetwork(bool a_virtualNetwork /*= true*/)
{
    this->setChildProperties(CHB_VIRTUAL_NETWORK, Properties::CHECKED, a_virtualNetwork);
}

#ifndef ANDROID
void MainScreen::setDownloadSpeed(double a_downloadSpeed)
{
    this->setChildProperties(LBL_DOWNLOADING_SPEED, Properties::TEXT, tr("%1 Mbps").arg(QString().number(a_downloadSpeed, 'f', 3)));
}

void MainScreen::setDownload(double a_download)
{
    this->setChildProperties(LBL_DOWNLOADED, Properties::TEXT, tr("%1 Mb").arg(QString().number(a_download, 'f', 3)));
}
#endif

void MainScreen::setBytesPackets(int a_bytesRead, int a_bytesWrite, int a_packetsRead, int a_packetsWrite)
{
    m_dataNew = {a_bytesRead, a_bytesWrite};

    QString readWrite;
    if(m_bytes_not_packets)
        readWrite = QString().number(a_bytesRead + a_bytesWrite);
    else
        readWrite = QString().number(a_packetsRead + a_packetsWrite);

    this->setChildProperties(LBL_RECEIVED, Properties::TEXT, readWrite);

#ifndef ANDROID
    setDownload(a_bytesRead);
#endif

}

void MainScreen::setTimeConnected(uint64_t a_timeConnected)
{
    this->setChildProperties(LBL_CONNECTED_TIME, Properties::TEXT, getUptime(a_timeConnected));
}

QString MainScreen::statusText(ConnectionStates a_state)
{
    switch (a_state)
    {
        case ConnectionStates::Disconnected:
            return  "Not connected";
        case ConnectionStates::Connecting:
            return "Connecting...";
        case ConnectionStates::Connected:
            return  "Connected";
        case ConnectionStates::Disconnecting:
            return  "Server down";
    }
    return QString();
}

QString MainScreen::getUptime(quint64 seconds)
{
    const qint64 DAY = 86400;
    qint64 days = seconds / DAY;
    QTime t = QTime(0,0).addSecs(seconds % DAY);
    QString res;
    res.sprintf("%d %02d:%02d:%02d", static_cast<int>(days), t.hour(), t.minute(), t.second());
    return res;
}

double MainScreen::calculateSpeedMbps()
{
    // Translation milliseconds to seconds
    ushort timeout = TIMEOUT_SPEED_CALCULATION/1000;
    double speedNew = 0.008*(m_dataNew.first - m_dataOld.first)/timeout;
    m_dataOld = m_dataNew;
    return speedNew;
}

void MainScreen::startCalculateConnectionData(const QDateTime &startTime)
{
    m_timeConnected = startTime.secsTo(QDateTime::currentDateTime());
    setTimeConnected(m_timeConnected);
    m_speedCalculationTimer.start();
    m_timeConnectedTimer.start();
}

void MainScreen::stopCalculationTimers()
{
    m_timeConnectedTimer.stop();
    m_speedCalculationTimer.stop();
    m_dataNew = m_dataOld = {0, 0};
}
