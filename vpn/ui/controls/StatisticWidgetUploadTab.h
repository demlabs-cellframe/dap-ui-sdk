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

    const QString UPLOAD_SPEED      = tr("Upload speed");
    const QString BYTES_SENT        = tr("Bytes sent");
    const QString PACKETS_SENT      = tr("Packets sent");
    const QString TIME_CONNECTED    = tr("Time connected");
};

#endif // STATISTICWIDGETUPLOADTAB_H
