#include "DapIndicatorTunnel.h"

void DapIndicatorTunnel::init(DapState *s, const QString& stateName) {
    DapIndicatorStateAbstract::init(s, stateName);
    tunDisabled = new DapState(_false->name()       + "_tunDisabled",   _false);
    tunEnabled  = new DapState(_true->name()        + "_tunEnabled",    _true);
    tunIdle     = new DapState(_trueToFalse->name() + "_tunIdle",       _trueToFalse);
    _false->setInitialState(tunDisabled);

    initAllowedSubstatesTransitions();
}

void DapIndicatorTunnel::initAllowedSubstatesTransitions() {
    addAllowedSubstatesTransitions(tunDisabled,     tunEnabled);
    addAllowedSubstatesTransitions(tunEnabled,      tunIdle);
    addAllowedSubstatesTransitions(tunIdle,         tunEnabled);
    addAllowedSubstatesTransitions(tunIdle,         tunDisabled);
    addAllowedSubstatesTransitions(tunEnabled,      tunDisabled);
}
