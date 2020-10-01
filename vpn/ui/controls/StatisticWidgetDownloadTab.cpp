#include "StatisticWidgetDownloadTab.h"

const QString StatisticWidgetDownloadTab::SCREEN_NAME = "StatisticWidgetDownloadTab";

StatisticWidgetDownloadTab::StatisticWidgetDownloadTab(QWidget *a_parrent)
    : AdaptiveScreen(a_parrent)
{
    this->create(m_ui);
}

QString StatisticWidgetDownloadTab::screenName()
{
    return StatisticWidgetDownloadTab::SCREEN_NAME;
}

void StatisticWidgetDownloadTab::setSpeed(QString a_speedStr)
{
    m_ui->lblSpeedValue->setText(a_speedStr);
}

void StatisticWidgetDownloadTab::setBytes(quint64 a_bytes)
{
    m_ui->lblBytesValue->setText(Utils::convertByte(a_bytes));
}

void StatisticWidgetDownloadTab::setPackets(quint64 a_packets)
{
    m_ui->lblPacketsValue->setText(QString::number(a_packets));
}

void StatisticWidgetDownloadTab::setTimeConnected(QDateTime a_time)
{
    QString connectedTime{};
    if (a_time.secsTo(QDateTime::currentDateTime()) == 0)
        connectedTime = "...";
    else
        connectedTime = toTimeString(a_time.secsTo(QDateTime::currentDateTime()));
    m_ui->lblTimeValue->setText(connectedTime);
}

void StatisticWidgetDownloadTab::setTitles(QString a_speed, QString a_bytes, QString a_packets, QString a_time)
{
    m_ui->lblSpeedTitle->setText(a_speed);
    m_ui->lblBytesTitle->setText(a_bytes);
    m_ui->lblPacketsTitle->setText(a_packets);
    m_ui->lblTimeTitle->setText(a_time);
}

QString StatisticWidgetDownloadTab::toTimeString(quint64 seconds)
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


void StatisticWidgetDownloadTab::initVariantUi(QWidget *a_widget)
{
    setTitles(DOWNLOAD_SPEED,BYTES_RECEIVED,PACKETS_RECEIVED,TIME_CONNECTED);
}

