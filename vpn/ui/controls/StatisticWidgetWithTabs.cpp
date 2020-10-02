#include "StatisticWidgetWithTabs.h"


const QString StatisticWidgetWithTabs::SCREEN_NAME = "StatisticWidgetWithTabs";

StatisticWidgetWithTabs::StatisticWidgetWithTabs(QWidget *a_parrent)
    : MultiScreenAbstract(a_parrent)
    , StatisticWidgetBase()
{
    this->create(m_ui);
}

QString StatisticWidgetWithTabs::screenName()
{
    return StatisticWidgetWithTabs::SCREEN_NAME;
}

void StatisticWidgetWithTabs::initVariantUi(QWidget *a_widget)
{
    this->initChangingWidget(a_widget);
    activateDownloadTab();

    connect(m_ui->btnUpload,&QPushButton::clicked,this,&StatisticWidgetWithTabs::activateUploadTab);
    connect(m_ui->btnDownload,&QPushButton::clicked,this,&StatisticWidgetWithTabs::activateDownloadTab);
}

void StatisticWidgetWithTabs::activateDownloadTab()
{
    this->activateScreen<StatisticWidgetDownloadTab>();

    m_ui->btnDownload->setChecked(true);
    m_ui->btnDownload->setEnabled(false);

    m_ui->btnUpload->setChecked(false);
    m_ui->btnUpload->setEnabled(true);
}

void StatisticWidgetWithTabs::activateUploadTab()
{
    this->activateScreen<StatisticWidgetUploadTab>();

    m_ui->btnUpload->setChecked(true);
    m_ui->btnUpload->setEnabled(false);

    m_ui->btnDownload->setChecked(false);
    m_ui->btnDownload->setEnabled(true);
}

void StatisticWidgetWithTabs::setSentReceivedQuantity(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent)
{
    uploadTab()->setBytes(a_bytesSent);
    downloadTab()->setBytes(a_bytesReceived);

    uploadTab()->setPackets(a_packetsSent);
    downloadTab()->setPackets(a_packetsReceived);
}

void StatisticWidgetWithTabs::setLoginTime(const QDateTime &loginTime)
{
    ///it's catching connecting time point and showing duration connecting
    uploadTab()->setTimeConnected(loginTime);
    downloadTab()->setTimeConnected(loginTime);
}

void StatisticWidgetWithTabs::setConnectingTimePoint(const QDateTime &loginTime)
{
    ///for what?
}

void StatisticWidgetWithTabs::setUploadSpeedText(const QString &a_speedString)
{
    uploadTab()->setSpeed(a_speedString);
}

void StatisticWidgetWithTabs::setDownloadSpeedText(const QString &a_speedString)
{
    downloadTab()->setSpeed(a_speedString);
}

StatisticWidgetDownloadTab *StatisticWidgetWithTabs::downloadTab()
{
    return this->createSubScreenIfNotExist<StatisticWidgetDownloadTab>();
}

StatisticWidgetDownloadTab *StatisticWidgetWithTabs::uploadTab()
{
    return this->createSubScreenIfNotExist<StatisticWidgetUploadTab>();
}
