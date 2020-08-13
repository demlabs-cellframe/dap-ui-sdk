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

    void setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent);

    void startConnectionTimer(const QDateTime &a_startTime);
    void stopConnectionTimer();

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<STATISTIC_FORM> m_ui;

    IndicatorsUnits m_indicatorUnits;
    int m_bytesSent       = 0;
    int m_bytesReceived   = 0;
    int m_packetsSent     = 0;
    int m_packetsReceived = 0;

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
