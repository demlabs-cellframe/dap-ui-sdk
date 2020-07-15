#include "DapIndicatorTunnel.h"

void DapIndicatorTunnel::init(QStateMachine &sm, const QString& stateName)
{
    DapIndicatorStateAbstract::init(sm, stateName);

    // Init substates false
    tunnelClosed           = new DapState(_false->name() + "Closed", _false);
    noTunnelError          = new DapState(_false->name() + "NoTunnelError", _false, true);
    noTunnelConfigError    = new DapState(_false->name() + "NoTunnelConfigError", _false, true);
    oldConfigRestoreError  = new DapState(_false->name() + "OldConfigRestoreError", _false, true);
    tunnelCloseError       = new DapState(_false->name() + "CloseError", _false, true);
    _false->setInitialState(tunnelClosed);

    // Init substates falseToTrue
    tunnelRequested   = new DapState(_falseToTrue->name() + "Requested", _falseToTrue);
    tunnelConfiguring = new DapState(_falseToTrue->name() + "Configuring", _falseToTrue);

    // Init substates True
    tunnelConfigured = new DapState(_true->name() + "Configured", _true);

    // Init substates TrueToFalse
    oldConfigRestoring = new DapState(_trueToFalse->name() + "OldConfigRestoring", _trueToFalse);
    tunnelClosing = new DapState(_trueToFalse->name() + "Closing", _trueToFalse);

    initAllowedSubstatesTransitions();
}

void DapIndicatorTunnel::initAllowedSubstatesTransitions()
{
    // False => FalseToTrue
    addAllowedSubstatesTransitions(tunnelClosed, tunnelConfiguring);
    addAllowedSubstatesTransitions(noTunnelError, tunnelConfiguring);
    addAllowedSubstatesTransitions(noTunnelConfigError, tunnelConfiguring);
    addAllowedSubstatesTransitions(oldConfigRestoreError, tunnelConfiguring);
    addAllowedSubstatesTransitions(tunnelCloseError, tunnelConfiguring);
    // FalseToTrue =>
    addAllowedSubstatesTransitions(tunnelConfiguring, tunnelConfigured);
    addAllowedSubstatesTransitions(tunnelConfiguring, noTunnelConfigError);

    // need add error state for tunnelConfigrud
    addAllowedSubstatesTransitions(tunnelConfigured, tunnelClosing);
    addAllowedSubstatesTransitions(tunnelConfiguring, tunnelClosing);

    // True =>
    addAllowedSubstatesTransitions(tunnelClosing, tunnelClosed);
}
