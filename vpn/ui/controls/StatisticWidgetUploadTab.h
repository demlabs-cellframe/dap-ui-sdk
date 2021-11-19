#ifndef STATISTICWIDGETUPLOADTAB_H
#define STATISTICWIDGETUPLOADTAB_H

#include "StatisticWidgetDownloadTab.h"

class StatisticWidgetUploadTab : public StatisticWidgetDownloadTab
{
    Q_OBJECT
public:
    StatisticWidgetUploadTab(QWidget* a_parrent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

private:
    void changeLabels();
};

#endif // STATISTICWIDGETUPLOADTAB_H
