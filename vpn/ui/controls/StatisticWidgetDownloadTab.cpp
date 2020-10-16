#include "StatisticWidgetDownloadTab.h"

const QString StatisticWidgetDownloadTab::SCREEN_NAME = "NavigationPanelBackBase";

StatisticWidgetDownloadTab::StatisticWidgetDownloadTab(QWidget *a_parrent)
    : AdaptiveScreen(a_parrent)
{
    ///TODO
    this->create(m_ui);
}

QString StatisticWidgetDownloadTab::screenName()
{
    return StatisticWidgetDownloadTab::SCREEN_NAME;
}

void StatisticWidgetDownloadTab::setSpeed(QString a_speedStr)
{
    ///TODO
}

void StatisticWidgetDownloadTab::setBytes()
{
    ///TODO
}

void StatisticWidgetDownloadTab::setPackets()
{
    ///TODO
}

void StatisticWidgetDownloadTab::setConnectingTimePoint()
{
    ///TODO
}


void StatisticWidgetDownloadTab::initVariantUi(QWidget *a_widget)
{
    ///TODO
}

