#ifndef STATISTICSCREENBASE_H
#define STATISTICSCREENBASE_H

#include "AdaptiveScreen.h"
#include "BrandAppProperties.h"

class StatisticScreenBase : public AdaptiveScreen
{
    Q_OBJECT
public:

    /// Overloaded constructor.
    /// @param a_parent Parent.
    StatisticScreenBase(QWidget *a_parent);

    static const QString SCREEN_NAME;
    virtual QString screenName() override;

public slots:

    void setAuthorized(bool a_authorized = true);
    void setStreamOpened(bool a_streamOpened = true);
    void setVirtualNetwork(bool a_virtualNetwork = true);

    void setSentReceivedIndicators(int a_bytesReceived, int a_bytesSent, int a_packetsReceived, int a_packetsSent);

protected:
    /// Form initialization.
    /// @param a_w Window GUI widget.
    virtual void initVariantUi(QWidget *a_widget) override;

    QScopedPointer<STATISTIC_FORM> m_ui;
};

#endif // CONNECTIONSCREENBASE_H
