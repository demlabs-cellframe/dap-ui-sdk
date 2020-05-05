#ifndef DAPINDICATORTUNNEL_H
#define DAPINDICATORTUNNEL_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorTunnel: public DapIndicatorStateAbstract
{
    Q_OBJECT
public:
    explicit DapIndicatorTunnel(QObject *parent = nullptr)
        : DapIndicatorStateAbstract(parent) { }
    void init(QStateMachine &sm, const QString& stateName) override;
protected:
    void initAllowedSubstatesTransitions() override;
public:
    // Substates False
    DapState *tunnelClosed;
    DapState *noTunnelError;
    DapState *noTunnelConfigError;
    DapState *oldConfigRestoreError;
    DapState *tunnelCloseError;

    // Substates falseToTrue
    DapState *tunnelRequested;
    DapState *tunnelConfiguring;

    // Substates True
    DapState *tunnelConfigured;

    // Substates TrueToFalse
    DapState *oldConfigRestoring;
    DapState *tunnelClosing;
};

#endif // DAPINDICATORTUNNEL_H
