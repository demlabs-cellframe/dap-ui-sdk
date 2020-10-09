#ifndef STATISTICWIDGETDOWNLOADTAB_H
#define STATISTICWIDGETDOWNLOADTAB_H

#include "AdaptiveScreen.h"
#include "ui_StatisticWidgetTab.h"

class StatisticWidgetDownloadTab: public AdaptiveScreen
{
    Q_OBJECT
public:
    StatisticWidgetDownloadTab(QWidget* a_parrent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    void setSpeed(QString a_speedStr);
    void setBytes();
    void setPackets();
    void setConnectingTimePoint();

protected:
    virtual void initVariantUi(QWidget *a_widget) override;

private:
    QScopedPointer<Ui::StatisticWidgetTab> m_ui;
};

#endif // STATISTICWIDGETDOWNLOADTAB_H
