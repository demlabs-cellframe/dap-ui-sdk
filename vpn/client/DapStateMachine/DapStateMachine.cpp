#include "DapStateMachine.h"
#include "DapCmdConnect.h"

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

    connect(&sessionStates, &DapIndicatorStateAbstract::stateChanged, [=](DapIndicator::State state) {
        _emitStateChanged(DapIndicator::Type::Session, state);
    });

    connect(&streamStates, &DapIndicatorStateAbstract::stateChanged, [=](DapIndicator::State state) {
        _emitStateChanged(DapIndicator::Type::Stream, state);
    });

    connect(&tunnelStates, &DapIndicatorStateAbstract::stateChanged, [=](DapIndicator::State state) {
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
}

void DapStateMachine::addUserRequestConnectNoCDB(const QObject *sender, const char *signal)
{
    userRequestStateDisconnect->addTransition(sender,signal,userRequestStateConnectNoCDB);
}

void DapStateMachine::_initUserRequestStates()
{
    userRequestStates = new DapState("statesRequest", initState);
    userRequestStateDisconnect = new DapState(userRequestStates->name() + "Disconnect", userRequestStates);
    userRequestStateConnect = new DapState(userRequestStates->name() + "Connect", userRequestStates);
    userRequestStateConnectNoCDB = new DapState(userRequestStates->name() + "ConnectNoCDB", userRequestStates);
    userRequestStates->setInitialState(userRequestStateDisconnect);
}
