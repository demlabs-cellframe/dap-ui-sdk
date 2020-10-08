#include "StatisticWidgetUploadTab.h"

const QString StatisticWidgetUploadTab::SCREEN_NAME = "NavigationPanelBackBase";

StatisticWidgetUploadTab::StatisticWidgetUploadTab(QWidget* a_parrent /*= nullptr*/)
    : StatisticWidgetDownloadTab(a_parrent)
{
    this->changeLabels();
}

void StatisticWidgetUploadTab::changeLabels()
{

}


QString StatisticWidgetUploadTab::screenName()
{
    return StatisticWidgetUploadTab::SCREEN_NAME;
}
