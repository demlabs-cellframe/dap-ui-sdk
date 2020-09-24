#include "StatisticWidgetWithTabs.h"


const QString StatisticWidgetWithTabs::SCREEN_NAME = "NavigationPanelBackBase";

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
    this->initChangingWidget();

    this->activateDownloadTab();
}

void StatisticWidgetWithTabs::activateDownloadTab()
{
    this->activateScreen<StatisticWidgetDownloadTab>();
}

void StatisticWidgetWithTabs::activateUploadTab()
{
    this->activateScreen<StatisticWidgetUploadTab>();
}


void StatisticWidgetWithTabs::setSentReceivedQuantity(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent)
{
}

void StatisticWidgetWithTabs::setLoginTime(const QDateTime &loginTime)
{
}

void StatisticWidgetWithTabs::setConnectingTimePoint(const QDateTime &loginTime)
{
}

void StatisticWidgetWithTabs::setUploadSpeedText(const QString &a_speedString)
{
}

void StatisticWidgetWithTabs::setDownloadSpeedText(const QString &a_speedString)
{
}

StatisticWidgetDownloadTab *StatisticWidgetWithTabs::downloadTab()
{
    return this->createSubScreenIfNotExist<StatisticWidgetDownloadTab>();
}

StatisticWidgetDownloadTab *StatisticWidgetWithTabs::uploadTab()
{
    return this->createSubScreenIfNotExist<StatisticWidgetUploadTab>();
}
