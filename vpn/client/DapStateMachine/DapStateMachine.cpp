#include "DapStateMachine.h"
#include "DapCmdConnect.h"

DapStateMachine::DapStateMachine(QObject *parent) : QObject(parent)
{
    // qDebug() << "DapStateMachine";
    sm.setChildMode(QState::ParallelStates);

    sessionStates.init(sm, "session");
    streamStates.init(sm, "stream");
    tunnelStates.init(sm, "tunnel");
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
    emit stateChanged(DapIndicator(type, state));
}

void DapStateMachine::addUserRequestDisconnectSignal(const QObject *sender, const char *signal)
{
    userRequestStateConnect->addTransition(sender,signal,userRequestStateDisconnect);
}

void DapStateMachine::addUserRequestConnect(const QObject *sender, const char *signal)
{
    userRequestStateDisconnect->addTransition(sender,signal,userRequestStateConnect);
}

void DapStateMachine::_initUserRequestStates()
{
    userRequestStates = new DapState("statesRequest", &sm);
    userRequestStateDisconnect = new DapState(userRequestStates->name() + "Disconnect", userRequestStates);

    userRequestStateConnect = new DapState(userRequestStates->name() + "Connect", userRequestStates);
    userRequestStates->setInitialState(userRequestStateDisconnect);

    /*userRequestStateConnect->addTransition(sessionStates.authRequestError, SIGNAL( entered() ), userRequestStateDisconnect );

    userRequestStateConnect->addTransition(sessionStates.networkError,
                                         SIGNAL(entered()),
                                         userRequestStateDisconnect);
    userRequestStateConnect->addTransition(sessionStates.handshakeError,
                                         SIGNAL(entered()),
                                         userRequestStateDisconnect);*/
}
