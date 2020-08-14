#include "StatisticScreenBase.h"

const QString StatisticScreenBase::SCREEN_NAME = "StatisticScreenBase";

StatisticScreenBase::StatisticScreenBase(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    //Add this to inheritable class if you want have working inharitance initVariantUi method:
    //this->create(m_ui);

    //And this:
    //AdaptiveScreen::initScreen(this);
    create(m_ui);

    AdaptiveScreen::initScreen(this);

    m_connectedTimer.setInterval(CONNECTED_TIME_INTERVAL);

    // Signal-slot connection that updates the total connection time with a timeout of 1 second
    connect(&m_connectedTimer, &QTimer::timeout, [this]
    {
        this->updateTimeIndicators();
    });

}

QString StatisticScreenBase::screenName()
{
    return StatisticScreenBase::SCREEN_NAME;
}

void StatisticScreenBase::setAuthorized(bool a_authorized /*= true*/)
{
    m_ui->chbAuthorized->setChecked(a_authorized);
}

void StatisticScreenBase::setStreamOpened(bool a_streamOpened /*= true*/)
{
    m_ui->chbStreamOpened->setChecked(a_streamOpened);
}

void StatisticScreenBase::setVirtualNetwork(bool a_virtualNetwork /*= true*/)
{
    m_ui->chbVirtualNetwork->setChecked(a_virtualNetwork);
}

void StatisticScreenBase::updateSentRecievedIndicators()
{
    //TODO:
    // Check why sigReadWriteBytesStat signal send wrong datas (bytes/packets are contrarily)
    m_ui->lblSent->setText(QString::number(m_bytesSent));
    m_ui->lblReceived->setText(QString::number(m_bytesReceived));
}

void StatisticScreenBase::setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent)
{
    m_bytesReceived   = a_bytesReceived;
    m_bytesSent       = a_bytesSent;
    m_packetsReceived = a_packetsReceived;
    m_packetsSent     = a_packetsSent;

    this->updateSentRecievedIndicators();

}

QDateTime StatisticScreenBase::loginTime() const
{
    return m_loginTime;
}

void StatisticScreenBase::setLoginTime(const QDateTime &loginTime)
{
    m_loginTime = loginTime;

    this->updateTimeIndicators();
}

StatisticScreenBase::IndicatorsUnits StatisticScreenBase::indicatorUnits() const
{
    return m_indicatorUnits;
}

void StatisticScreenBase::updateTimeIndicators()
{
    QString loginTime;
    if (m_loginTime == QDateTime())
        loginTime = EMPTY_TYME;
    else if (m_loginTime.date() == QDate::currentDate())
        loginTime = m_loginTime.toString("hh:mm");
    else
        loginTime = m_loginTime.toString("MM-dd-yy hh:mm");

    m_ui->lblLoginTime->setText(loginTime);

    QString connectedTime;
    if (this->connectedTime() == 0)
        connectedTime = EMPTY_TYME;
    else
        connectedTime = StatisticScreenBase::toTimeString(this->connectedTime());

    m_ui->lblConnectedTime->setText(connectedTime);
}

uint64_t StatisticScreenBase::connectedTime()
{
    return m_loginTime.secsTo(QDateTime::currentDateTime());
}

QString StatisticScreenBase::toTimeString(quint64 seconds)
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

void StatisticScreenBase::startConnectionTimer(const QDateTime &a_startTime)
{
    this->setLoginTime(a_startTime);

    m_connectedTimer.start();
}

void StatisticScreenBase::stopConnectionTimer()
{
    this->setLoginTime(QDateTime());

    m_connectedTimer.stop();
}


void StatisticScreenBase::initVariantUi(QWidget *a_widget)
{
    Q_UNUSED(a_widget)
}
