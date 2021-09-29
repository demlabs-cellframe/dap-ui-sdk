#pragma once
#include <QObject>
#include <QDebug>
#include <QState>
#include <QStateMachine>

#include <QCache>
#include <QVector>

#include "DapIndicator.h"
#include "DapState.h"
#include "DapIndicatorStream.h"
#include "DapIndicatorTunnel.h"
#include "DapIndicatorSession.h"

class DapStateMachine : public QObject
{
    Q_OBJECT
private:
    explicit DapStateMachine(QObject *parent = nullptr);
public:
    static DapStateMachine* instance() {
        static DapStateMachine _me;
        return &_me;
    }

    DapIndicatorSession sessionStates;
    DapIndicatorStream streamStates;
    DapIndicatorTunnel tunnelStates;

    // state mirrored what want user (connect or disconnect)
    void addUserRequestDisconnectSignal(const QObject *sender, const char *signal);
    void addUserRequestConnect(const QObject *sender, const char *signal);

    //
    DapState *userRequestStates;
    DapState *userRequestStateConnect;
    DapState *userRequestStateDisconnect;

    DapState *initState;

    const QVector<DapIndicator>& getCachedStates();
signals:
    void stateChanged(DapIndicator i);
private:
    /* state machine have parallel states: statesRequest and
     * indicator states: Initialization, Stream, Tunnel */
    QStateMachine sm;

    // Cache all states
    QCache<DapIndicator::Type, DapIndicator::State> m_indicatorStateCache;
    QVector<DapIndicator> _statesBuffer;

    void _initUserRequestStates();
    void _emitStateChanged(DapIndicator::Type t, DapIndicator::State s);
};
