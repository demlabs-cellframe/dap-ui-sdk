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

//    no_cdbMode          = new DapState(_false->name()   + "no_cdbMode", _false);
    no_cdbModeConnect   = new DapState(_true->name()    + "no_cdbModeConnect", _true);
    no_cdbDisconnect    = new DapState(_false->name()   + "no_cdbDisconnect", _false);

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


    addAllowedSubstatesTransitions(cdbLogouted,             no_cdbModeConnect);
//    addAllowedSubstatesTransitions(no_cdbMode,              no_cdbModeConnect);
    addAllowedSubstatesTransitions(no_cdbModeConnect,       no_cdbDisconnect);
    addAllowedSubstatesTransitions(no_cdbDisconnect,        cdbLogouted);
}
