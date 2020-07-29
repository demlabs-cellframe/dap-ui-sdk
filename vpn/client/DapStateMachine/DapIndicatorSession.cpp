#include "DapIndicatorSession.h"

void DapIndicatorSession::init(QStateMachine &sm, const QString& stateName)
{
    DapIndicatorStateAbstract::init(sm, stateName);

    // Init false substates
    logouted         = new DapState(_false->name() + "Logouted", _false);
    authRequestError = new DapState(_false->name() + "AuthError", _false, true);
    streamCtlError   = new DapState(_false->name() + "StreamCtlError", _false, true);
    handshakeError   = new DapState(_false->name() + "HandshakeError", _false, true);
    logoutError      = new DapState(_false->name() + "LogoutError", _false, true);
    networkError     = new DapState(_false->name() + "NetworkError", _false, true);
    networkErrorIdle = new DapState(_false->name() + "NetworkErrorIdle", _false, true);

    handshakeRequestCanceling = new DapState(_falseToTrue->name() + "SessionRequestCanceling", _false);
    streamRequestCanceling    = new DapState(_falseToTrue->name() + "StreamRequestCanceling", _false);
    authRequestCanceling      = new DapState(_falseToTrue->name() + "AuthRequestCanceling", _false);
    authCanceling             = new DapState(_falseToTrue->name() + "AuthCanceling", _false);

    _false->setInitialState(logouted);

    // Init falseToTrue substates
    handshakeRequested  = new DapState(_falseToTrue->name() + "HandshakeRequested",
                                      _falseToTrue);
    handshakeResponse   = new DapState(_falseToTrue->name() + "HandshakeResponse",
                                       _falseToTrue);
    authRequested       = new DapState(_falseToTrue->name() + "AuthRequested",
                                       _falseToTrue);
    authorized          = new DapState(_falseToTrue->name() + "Authorized",
                                       _falseToTrue);
    streamCtlRequested  = new DapState(_falseToTrue->name() + "StreamCtlRequested",
                                       _falseToTrue);
    keyActivationRequested = new DapState(_falseToTrue->name() + "KeyActivationRequested",
                                          _falseToTrue);
    keyActivated        = new DapState(_falseToTrue->name() + "KeyActivated",
                                       _falseToTrue);
    // Init true substates
    streamSessionOpened = new DapState(_true->name() + "StreamOpened", _true);

    // Init trueToFalse substates
    logoutRequested = new DapState(_trueToFalse->name() + "LogoutRequsted",   _trueToFalse);
    unAuthorized = new DapState(_trueToFalse->name() + "UnAuthorized", _trueToFalse, true);

    //switchRequested = new DapState(_trueToFalse->name() + "switchRequested", _trueToFalse);

    initAllowedSubstatesTransitions();
}

void DapIndicatorSession::initAllowedSubstatesTransitions()
{
    /* Init allowed false substates transitions */

    // logouted =>
    addAllowedSubstatesTransitions(logouted, handshakeRequested);
    addAllowedSubstatesTransitions(authRequestError, handshakeRequested);
    addAllowedSubstatesTransitions(streamCtlError, handshakeRequested);
    addAllowedSubstatesTransitions(handshakeError, handshakeRequested);
    addAllowedSubstatesTransitions(logoutError, handshakeRequested);
    addAllowedSubstatesTransitions(networkError, handshakeRequested);
    //addAllowedSubstatesTransitions(networkErrorIdle, handshakeRequested);

    /* Init allowed falseToTrue substates  transitions */
    // _handshakeRequested =>
    addAllowedSubstatesTransitions(handshakeRequested, handshakeResponse);
    addAllowedSubstatesTransitions(handshakeRequested, handshakeError);
    //addAllowedSubstatesTransitions(handshakeRequested, networkErrorIdle);
    addAllowedSubstatesTransitions(handshakeRequested, networkError);
    addAllowedSubstatesTransitions(handshakeRequested, handshakeRequestCanceling);

    // handshakeRequestCanceling =>
    addAllowedSubstatesTransitions(handshakeRequestCanceling, logouted);

    // authRequestCanceling =>
    addAllowedSubstatesTransitions(authRequestCanceling, logouted);

    // HandshakeResponse =>
    addAllowedSubstatesTransitions(handshakeResponse, authRequested);
    addAllowedSubstatesTransitions(handshakeResponse, handshakeRequestCanceling);
    addAllowedSubstatesTransitions(handshakeResponse, streamCtlRequested);
    addAllowedSubstatesTransitions(handshakeResponse, streamCtlError);
    addAllowedSubstatesTransitions(handshakeResponse, handshakeError);

    // _authRequested =>
    addAllowedSubstatesTransitions(authRequested, authorized);
    addAllowedSubstatesTransitions(authRequested, keyActivationRequested);
    addAllowedSubstatesTransitions(keyActivationRequested, keyActivated);
    addAllowedSubstatesTransitions(keyActivated, authRequested);
    // error processing...
    addAllowedSubstatesTransitions(authRequested, authRequestCanceling);
    addAllowedSubstatesTransitions(authRequested, authRequestError);
    addAllowedSubstatesTransitions(keyActivationRequested, authRequestError);
    addAllowedSubstatesTransitions(keyActivated, authRequestError);
    // _authorized =>
    addAllowedSubstatesTransitions(authorized, authCanceling);
    addAllowedSubstatesTransitions(authorized, handshakeRequested);

    addAllowedSubstatesTransitions(authCanceling, logouted);
    addAllowedSubstatesTransitions(authCanceling, logoutError);

    // _streamCtlRequested =>
    addAllowedSubstatesTransitions(streamCtlRequested, streamSessionOpened);
    addAllowedSubstatesTransitions(streamCtlRequested, streamCtlError);
    addAllowedSubstatesTransitions(streamCtlRequested, streamRequestCanceling);

    // handshakeRequestCanceling =>
    addAllowedSubstatesTransitions(streamRequestCanceling, logouted);
    addAllowedSubstatesTransitions(streamRequestCanceling, logoutError);

    /* Init allowed true substates transitions */
    // _streamSessionOpened =>
    addAllowedSubstatesTransitions(streamSessionOpened, logoutRequested);
    addAllowedSubstatesTransitions(streamSessionOpened, unAuthorized);
    addAllowedSubstatesTransitions(streamSessionOpened, handshakeRequested);

    //addAllowedSubstatesTransitions(streamSessionOpened, switchRequested);
    //addAllowedSubstatesTransitions(switchRequested, handshakeRequested);

    // TrueToFalse =>
    addAllowedSubstatesTransitions(logoutRequested, logoutError);
    addAllowedSubstatesTransitions(logoutRequested, logouted);
    addAllowedSubstatesTransitions(unAuthorized, logouted);
}
