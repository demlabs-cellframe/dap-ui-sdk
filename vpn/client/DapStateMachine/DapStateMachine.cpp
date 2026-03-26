#include "DapStateMachine.h"

DapStateMachine::DapStateMachine(QObject *parent) : QObject(parent), sm(this)
{
    initState = new DapState("initial", &sm);
    initState->setChildMode(QState::ParallelStates);
    sm.setInitialState(initState);
    sessionStates.init(initState, "session");
    streamStates.init(initState, "stream");
    tunnelStates.init(initState, "tunnel");
    _initUserRequestStates();

    _statesBuffer.resize(DapIndicator::TYPE_COUNT);

    connect(&sessionStates, &DapIndicatorStateAbstract::stateChanged, this, [this](DapIndicator::State state) {
        _emitStateChanged(DapIndicator::Type::Session, state);
    });

    connect(&streamStates, &DapIndicatorStateAbstract::stateChanged, this, [this](DapIndicator::State state) {
        _emitStateChanged(DapIndicator::Type::Stream, state);
    });

    connect(&tunnelStates, &DapIndicatorStateAbstract::stateChanged, this,  [this](DapIndicator::State state) {
        _emitStateChanged(DapIndicator::Type::Tunnel, state);
    });

    sm.start();
}

const QVector<DapIndicator>& DapStateMachine::getCachedStates()
{
    _statesBuffer.clear();
    for(const auto type : m_indicatorStateCache.keys()) {
       _statesBuffer.append(DapIndicator(type, *m_indicatorStateCache.object(type)));
    }
    return _statesBuffer;
}

const QJsonObject DapStateMachine::getJsonCachedStates()
{
    QJsonObject statesInfo;
    for (const auto &state: getCachedStates()) {
        statesInfo.insert(state.getStringType(), state.getStringState());
        qDebug() << "[DapChainVpnService] [onIndicatorStateChanged] type: " << state.getStringType() << ", state: " << state.getStringState();
    }
    return statesInfo;
}

void DapStateMachine::_emitStateChanged(DapIndicator::Type type, DapIndicator::State state)
{
    m_indicatorStateCache.insert(type, new DapIndicator::State(state));
    qInfo() << "~ State changed. Type: " + DapIndicator(type, state).getStringType() + " State: " + DapIndicator(type, state).getStringState();
    emit stateChanged(DapIndicator(type, state));
}

void DapStateMachine::addUserRequestDisconnectSignal(const QObject *sender, const char *signal)
{
    userRequestStateConnect->addTransition(sender,signal,userRequestStateDisconnect);
    userRequestStateConnectNoCDB->addTransition(sender,signal,userRequestStateDisconnect);
    userRequestStateError->addTransition(sender,signal,userRequestStateDisconnect);

    connect (this->userRequestStateConnect, &DapState::entered, [] {
        qDebug() << "DapStateMachine::addUserRequestDisconnectSignal - userRequestStateConnect";
    });

    connect (this->userRequestStateDisconnect, &DapState::entered, [] {
        qDebug() << "DapStateMachine::addUserRequestDisconnectSignal - userRequestStateDisconnect";
    });

    connect (this->userRequestStateConnectNoCDB, &DapState::entered, [] {
        qDebug() << "DapStateMachine::addUserRequestDisconnectSignal - userRequestStateConnectNoCDB";
    });
}

void DapStateMachine::addUserRequestConnect(const QObject *sender, const char *signal)
{
    userRequestStateDisconnect->addTransition(sender,signal,userRequestStateConnect);
    userRequestStateError->addTransition(sender,signal,userRequestStateConnect);
}

void DapStateMachine::addUserRequestConnectNoCDB(const QObject *sender, const char *signal)
{
    userRequestStateDisconnect->addTransition(sender,signal,userRequestStateConnectNoCDB);
    userRequestStateError->addTransition(sender,signal,userRequestStateConnectNoCDB);
}

void DapStateMachine::addUserRequestErrorSignal(const QObject *sender, const char *signal)
{
    userRequestStateConnect->addTransition(sender, signal, userRequestStateError);
    userRequestStateConnectNoCDB->addTransition(sender, signal, userRequestStateError);
}

bool DapStateMachine::isTunnelActive() const
{
    // Use cached indicator state instead of live QStateMachine sub-state
    // to avoid race: tunnelFalse.entered fires before tunDisabled.entered,
    // so tunDisabled->active() may return false when tunnel is already "false"
    auto *cachedState = m_indicatorStateCache.object(DapIndicator::Type::Tunnel);
    if(!cachedState)
        return false;
    return *cachedState != DapIndicator::State::False;
}

void DapStateMachine::_initUserRequestStates()
{
    userRequestStates = new DapState("statesRequest", initState);
    userRequestStateDisconnect = new DapState(userRequestStates->name() + "Disconnect", userRequestStates);
    userRequestStateConnect = new DapState(userRequestStates->name() + "Connect", userRequestStates);
    userRequestStateConnectNoCDB = new DapState(userRequestStates->name() + "ConnectNoCDB", userRequestStates);
    userRequestStateError = new DapState(userRequestStates->name() + "Error", userRequestStates);
    userRequestStates->setInitialState(userRequestStateDisconnect);
}
