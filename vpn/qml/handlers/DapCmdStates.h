#ifndef DAPSTATESHANDLER_H
#define DAPSTATESHANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include "DapIndicator.h"
#include "DapCmdClientAbstract.h"

using IndicatorState = DapIndicator::State;

class DapCmdStates: public DapCmdClientAbstract
{
    Q_OBJECT
protected:
    void handleResult(const QJsonObject& result) override;
    void handleError(int code, const QString& message) override;
public:
    explicit DapCmdStates(QObject *parent = nullptr);
    ~DapCmdStates() override;

    void sendCmd();

    DapIndicator *sessionIndicator() const;
    DapIndicator *streamIndicator() const;
    DapIndicator *tunnelIndicator() const;

private:
    using IndicatorType = DapIndicator::Type;

    static const QString stateNameParam;
    void sessionHandler(IndicatorState state);
    void tunnelHandler(IndicatorState state);
    void streamHandler(IndicatorState state);

    void userHandler(const QString& state);
    void serverChangedHandler(const QString& state);

    DapIndicator *m_stream;
    DapIndicator *m_session;
    DapIndicator *m_tunnel;
    bool m_tunnelActive = false;

    bool allStatesIsTrue();
    bool allStatesIsFalse();
signals:
    /* Auth signals group*/
    void sigAuthorizing();
    void sigAuthorized();
    void sigUnauthorizing();
    void sigUnauthorized();

    /* Tunnel signals group */
    void sigTunnelCreated();
    void sigTunnelDestroyed();
    void sigTunnelCreating();
    void sigTunnelDestroying();

    /* Stream signalns group */
    void sigStreamOpened();
    void sigStreamOpening();
    void sigStreamClosed();
    void sigStreamClosing();

    void sigAllIndicatorStatesIsFalse();
    void sigAllIndicatorStatesIsTrue();

    void sigConnectionLostWithTunnel();
    void sigServiceConnectionState(const QString& state);
    void sigServiceAlreadyConnected();

    void sigUserStateConnect();
    void sigUserStateDisconnect();
    void sigUserState(const QString& state);
    void sigNewState(const QJsonObject& result);

    /*Changing server for mainScreen*/
    void sigServerChanged();
    void sigServerNotChanged();
public slots:
};

#endif // DAPSTATESHANDLER_H
