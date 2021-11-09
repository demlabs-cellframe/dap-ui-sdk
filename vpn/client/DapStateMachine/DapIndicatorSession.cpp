#include "DapIndicatorSession.h"

void DapIndicatorSession::init(DapState *s, const QString& stateName)
{
    DapIndicatorStateAbstract::init(s, stateName);

    cdbLogouted         = new DapState(_false->name()       + "_cdbLogouted",       _false);
    networkErr          = new DapState(_false->name()       + "_networkErr",        _false, true);
    cdbLogoutRequest    = new DapState(_trueToFalse->name() + "_logoutRequest",     _true);
    cdbHandshakeRequest = new DapState(_falseToTrue->name() + "_cdbHSRequested",    _falseToTrue);
    cdbHandshakeReply   = new DapState(_falseToTrue->name() + "_cdbHSReplied",      _falseToTrue);
    cdbLogined          = new DapState(_true->name()        + "_cdbLogined",        _true);
    cdbKeyActivation    = new DapState(_falseToTrue->name() + "_cdbKeyActivation",  _falseToTrue);

    _false->setInitialState(cdbLogouted);
    initAllowedSubstatesTransitions();
}

void DapIndicatorSession::initAllowedSubstatesTransitions() {
    addAllowedSubstatesTransitions(cdbLogouted,             cdbHandshakeRequest);
    addAllowedSubstatesTransitions(cdbHandshakeRequest,     cdbHandshakeReply);
    addAllowedSubstatesTransitions(cdbHandshakeRequest,     networkErr);
    addAllowedSubstatesTransitions(cdbHandshakeReply,       cdbKeyActivation);
    addAllowedSubstatesTransitions(cdbHandshakeReply,       cdbLogined);
    addAllowedSubstatesTransitions(cdbHandshakeReply,       cdbLogouted);
    addAllowedSubstatesTransitions(cdbHandshakeReply,       networkErr);
    addAllowedSubstatesTransitions(cdbHandshakeReply,       cdbHandshakeRequest);
    addAllowedSubstatesTransitions(networkErr,              cdbLogouted);
    addAllowedSubstatesTransitions(networkErr,              cdbHandshakeRequest);
    addAllowedSubstatesTransitions(cdbKeyActivation,        cdbHandshakeReply);
    addAllowedSubstatesTransitions(cdbKeyActivation,        cdbLogouted);
    addAllowedSubstatesTransitions(cdbLogined,              cdbLogoutRequest);
    addAllowedSubstatesTransitions(cdbLogoutRequest,        cdbLogouted);
}
