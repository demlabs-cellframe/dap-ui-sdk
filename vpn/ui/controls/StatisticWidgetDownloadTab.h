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
    void setBytes(quint64 a_bytes);
    void setPackets(quint64 a_packets);
    void setTimeConnected(QDateTime a_time);

protected:
    virtual void initVariantUi(QWidget *a_widget) override;
    void setTitles(QString a_speed, QString a_bytes,QString a_packets, QString a_time);

private:
    QString toTimeString(quint64 seconds);
    QScopedPointer<Ui::StatisticWidgetTab> m_ui;

    const QString DOWNLOAD_SPEED    = tr("Download speed");
    const QString BYTES_RECEIVED    = tr("Bytes received");
    const QString PACKETS_RECEIVED  = tr("Packets received");
    const QString TIME_CONNECTED    = tr("Time connected");
};

#endif // STATISTICWIDGETDOWNLOADTAB_H
