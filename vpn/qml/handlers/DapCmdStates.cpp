#include "DapCmdStates.h"
#include <QDebug>

const QString DapCmdStates::stateNameParam = DapJsonParams::toString(DapJsonParams::STATE_NAME);

DapCmdStates::DapCmdStates(QObject *parent)
    : DapCmdClientAbstract(DapJsonCmdType::GET_STATES, parent)
{
    m_stream  = new DapIndicator(IndicatorType::Stream,
                                 IndicatorState::False);

    m_session = new DapIndicator(IndicatorType::Session,
                                 IndicatorState::False);

    m_tunnel  = new DapIndicator(IndicatorType::Tunnel,
                                 IndicatorState::False);
}

void DapCmdStates::sendCmd()
{
    DapCmdClientAbstract::sendCmd();
}

DapIndicator *DapCmdStates::sessionIndicator() const
{
    return m_session;
}

DapIndicator *DapCmdStates::streamIndicator() const
{
    return m_stream;
}

DapIndicator *DapCmdStates::tunnelIndicator() const
{
    return m_tunnel;
}

DapCmdStates::~DapCmdStates()
{

}

bool DapCmdStates::allStatesIsTrue()
{
  auto trueToFalseorTrue = [] (IndicatorState a_state)
  {
    return a_state == IndicatorState::True;
  };

  bool sessionState = trueToFalseorTrue (m_session->getState());
  bool tunnelState  = trueToFalseorTrue (m_tunnel->getState());
  bool streamState  = trueToFalseorTrue (m_stream->getState());

  return sessionState && tunnelState && streamState;
}

bool DapCmdStates::allStatesIsFalse()
{
    return  m_session->getState() == IndicatorState::False &&
            m_tunnel->getState() == IndicatorState::False &&
            m_stream->getState() == IndicatorState::False;
}

void DapCmdStates::handleError(int code, const QString& message)
{
    Q_UNUSED(code); Q_UNUSED(message);
    qWarning() << *m_errorObject;
}

void DapCmdStates::userHandler(const QString& state)
{
    emit sigUserState(state);
    if(state == "Disconnect") {
        emit sigUserStateDisconnect();
    } else if(state == "Connect") {
        emit sigUserStateConnect();
    } else {
        qCritical() << "Unknown user state";
    }
}

void DapCmdStates::serverChangedHandler(const QString& state)
{
    if(state == "ServerChanged") {
        emit sigServerChanged();
    } else if(state == "ServerNotChanged") {
        emit sigServerNotChanged();
    } else {
        qCritical() << "Unknown server changed state";
    }
}

void DapCmdStates::handleResult(const QJsonObject& result)
{
    static QMap<QString, void(DapCmdStates::*)(IndicatorState)> stateCallbacks = {
        {"session", &DapCmdStates::sessionHandler},
        {"tunnel", &DapCmdStates::tunnelHandler},
        {"stream", &DapCmdStates::streamHandler}
    };
    qDebug() << "Call stateHandler" << result;
    QString userRequest = QStringLiteral("user_request_state");
    QString serverChange = QStringLiteral("server_change_state");

    if(result.contains(QStringLiteral("tunnel_active")))
    {
        m_tunnelActive = result.value(QStringLiteral("tunnel_active")).toBool();
    }

    if(result.contains("states")){
        QJsonObject states = result.value("states").toObject();

        emit sigNewState(states);

        // Update all indicator states first, then check aggregate signals.
        // Checking inside the loop causes false positives because states
        // processed later still hold their old values during earlier checks.
        for(const auto& stateName: states.keys())
        {
            if(!stateCallbacks.contains(stateName)) {
                qWarning() << "Not found handler for " << stateName;
                continue;
            }
            IndicatorState state = DapIndicator::fromString(states.value(stateName).toString());
            (this->*stateCallbacks[stateName])(state);
        }

        if(allStatesIsTrue()) {
            qDebug() << "emit AllStatesIsTrue signal";
            emit sigAllIndicatorStatesIsTrue();
        } else if(allStatesIsFalse()) {
            if(m_tunnelActive) {
                qDebug() << "emit sigConnectionLostWithTunnel signal";
                emit sigConnectionLostWithTunnel();
            } else {
                qDebug() << "emit sigAllStatesIsFalse signal";
                emit sigAllIndicatorStatesIsFalse();
            }
        }
    }
    if(result.contains(userRequest)){
        userHandler(result.value(userRequest).toString());
    }

    if(result.contains(serverChange)){
        serverChangedHandler(result.value(serverChange).toString());
    }

    if(result.contains(QStringLiteral("connection_state")))
    {
        QString connState = result.value(QStringLiteral("connection_state")).toString();
        emit sigServiceConnectionState(connState);
        if(connState == QStringLiteral("CONNECT"))
        {
            emit sigServiceAlreadyConnected();
        }
    }
}

void DapCmdStates::sessionHandler(IndicatorState state)
{
    m_session->setState(state);
    switch (state) {
    case IndicatorState::False:
        emit sigUnauthorized();
        return;
    case IndicatorState::FalseToTrue:
        emit sigAuthorizing();
        return;
    case IndicatorState::True:
        emit sigAuthorized();
        return;
    case IndicatorState::TrueToFalse:
        emit sigUnauthorizing();
        return;
    }
    throw std::runtime_error("Can't handle Authorize state");
}

void DapCmdStates::tunnelHandler(IndicatorState state)
{
    m_tunnel->setState(state);
    switch (state) {
    case IndicatorState::False:
        emit sigTunnelDestroyed();
        return;
    case IndicatorState::FalseToTrue:
        emit sigTunnelCreating();
        return;
    case IndicatorState::True:
        emit sigTunnelCreated();
        return;
    case IndicatorState::TrueToFalse:
        emit sigTunnelDestroying();
        return;
    }
    throw std::runtime_error("Can't handle Tunnel state");
}

void DapCmdStates::streamHandler(IndicatorState state)
{
    m_stream->setState(state);
    switch (state) {
    case IndicatorState::False:
        emit sigStreamClosed();
        return;
    case IndicatorState::FalseToTrue:
        emit sigStreamOpening();
        return;
    case IndicatorState::True:
        emit sigStreamOpened();
        return;
    case IndicatorState::TrueToFalse:
        emit sigStreamClosing();
        return;
    }
    throw std::runtime_error("Can't handle stream state");
}
