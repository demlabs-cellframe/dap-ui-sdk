#include "StatisticWidgetUploadTab.h"

const QString StatisticWidgetUploadTab::SCREEN_NAME = "StatisticWidgetUploadTab";

StatisticWidgetUploadTab::StatisticWidgetUploadTab(QWidget* a_parrent /*= nullptr*/)
    : StatisticWidgetDownloadTab(a_parrent)
{
    this->changeLabels();
}

void StatisticWidgetUploadTab::changeLabels()
{
    this->setTitles(UPLOAD_SPEED,BYTES_SENT,PACKETS_SENT,TIME_CONNECTED);
}


QString StatisticWidgetUploadTab::screenName()
{
    return StatisticWidgetUploadTab::SCREEN_NAME;
}
