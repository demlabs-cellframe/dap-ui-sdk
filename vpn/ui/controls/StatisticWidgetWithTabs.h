#ifndef STATISTICWIDGETWITHTABS_H
#define STATISTICWIDGETWITHTABS_H

#include "MultiScreenAbstract.h"
#include "StatisticWidgetBase.h"
#include "StatisticWidgetDownloadTab.h"
#include "StatisticWidgetUploadTab.h"


#include "ui_StatisticWidgetWithTabs.h"

class StatisticWidgetWithTabs : public MultiScreenAbstract, StatisticWidgetBase
{
    Q_OBJECT
public:
    StatisticWidgetWithTabs(QWidget* a_parrent = nullptr);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    // AdaptiveWidget interface
protected:
    virtual void initVariantUi(QWidget *a_widget) override;

protected slots:
    void activateDownloadTab();
    void activateUploadTab();


    // StatisticFrameBase interface
public:
    virtual void setSentReceivedQuantity(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent) override;
    virtual void setLoginTime(const QDateTime &loginTime) override;
    virtual void setConnectingTimePoint(const QDateTime &loginTime) override;
    virtual void setUploadSpeedText(const QString &a_speedString) override;
    virtual void setDownloadSpeedText(const QString &a_speedString) override;

private:

    StatisticWidgetDownloadTab* downloadTab();
    StatisticWidgetDownloadTab *uploadTab();

    QScopedPointer<Ui::StatisticWidgetWithTabs> m_ui;
};

#endif // STATISTICWIDGETWITHTABS_H
