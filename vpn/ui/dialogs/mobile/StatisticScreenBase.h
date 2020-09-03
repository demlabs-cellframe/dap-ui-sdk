#ifndef STATISTICSCREENBASE_H
#define STATISTICSCREENBASE_H

#include "AdaptiveScreen.h"
#include "BrandAppProperties.h"

#include <QTimer>

class StatisticScreenBase : public AdaptiveScreen
{
    Q_OBJECT
public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    StatisticScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

    enum class IndicatorsUnits {Bytes, Packets};

    IndicatorsUnits indicatorUnits() const;

public slots:

    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);

    void setSentReceivedIndicators(quint64 a_bytesReceived, quint64 a_bytesSent, quint64 a_packetsReceived, quint64 a_packetsSent, QString a_uploadSpeed, QString a_downloadSpeed);

    void startConnectionTimer(const QDateTime &a_startTime);
    void stopConnectionTimer();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<STATISTIC_FORM> m_ui;

    IndicatorsUnits m_indicatorUnits;
    quint64 m_bytesSent         = 0;
    quint64 m_bytesReceived     = 0;
    quint64 m_packetsSent       = 0;
    quint64 m_packetsReceived   = 0;
    QString m_uploadSpeed       = "0 bps";
    QString m_downloadSpeed     = "0 bps";

    void updateSentRecievedIndicators();
    bool indicatorUnitsIsBytes() const;
    void updateTimeIndicators();

    QDateTime m_loginTime {};
    const QString EMPTY_TYME          = "...";
    uint64_t connectedTime();

    static QString toTimeString(quint64 seconds);

    QDateTime loginTime() const;
    void setLoginTime(const QDateTime &loginTime);

    QTimer  m_connectedTimer;
    const ushort CONNECTED_TIME_INTERVAL{1000};
};

#endif // CONNECTIONSCREENBASE_H
