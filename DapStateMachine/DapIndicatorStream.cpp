#include "DapIndicatorStream.h"

void DapIndicatorStream::init(QStateMachine &sm, const QString& stateName)
{
    DapIndicatorStateAbstract::init(sm, stateName);

    // Init substates false
    disconnectedNormal = new DapState(_false->name() + "DisconnectedNormal", _false);
    disconnectedError  = new DapState(_false->name() + "DisconnectedError", _false, true);
    ipRequestError     = new DapState(_false->name() + "RequestError", _false, true);
    serviceError       = new DapState(_false->name() + "ServiceError", _false, true);

    _false->setInitialState(disconnectedNormal);

    // Init substates falseToTrue
    connected          = new DapState(_falseToTrue->name() + "Connected",
                                       _falseToTrue);
    ipRequested        = new DapState(_falseToTrue->name() + "IpRequested",
                                       _falseToTrue);
    connectError       = new DapState(_falseToTrue->name() + "ConnectError",
                                       _falseToTrue, true);
    reconnecting       = new DapState(_falseToTrue->name() + "Reconnecting",
                                       _falseToTrue);
    reconnectingError  = new DapState(_falseToTrue->name() + "ReconnectingError",
                                       _falseToTrue, true);
    serviceRequested   = new DapState(_falseToTrue->name() + "ServiceRequested",
                                      _falseToTrue);
    serviceSuccess     = new DapState(_falseToTrue->name() + "ServiceSuccess",
                                      _falseToTrue);
    // Init substates True
    addressReceived = new DapState(_true->name() + "AddressReceived",
                                         _true);

    // Init substates TrueToFalse
    disconnecting      = new DapState(_trueToFalse->name() + "Disconnecting",
                                            _trueToFalse);
    initAllowedSubstatesTransitions();
}

void DapIndicatorStream::initAllowedSubstatesTransitions()
{
    // False
    addAllowedSubstatesTransitions(disconnectedNormal, connected);
    addAllowedSubstatesTransitions(disconnectedError, connected);
    addAllowedSubstatesTransitions(ipRequestError, connected);

    // FalseToTrue => *
    //addAllowedSubstatesTransitions(connected, ipRequested);
    addAllowedSubstatesTransitions(connected, serviceRequested);
    addAllowedSubstatesTransitions(serviceRequested, serviceSuccess);
    addAllowedSubstatesTransitions(serviceRequested, serviceError);
    addAllowedSubstatesTransitions(serviceSuccess, ipRequested);
    addAllowedSubstatesTransitions(ipRequested, addressReceived);
    addAllowedSubstatesTransitions(ipRequested, ipRequestError);
    addAllowedSubstatesTransitions(reconnecting, connected);
    addAllowedSubstatesTransitions(reconnecting, reconnectingError);
    addAllowedSubstatesTransitions(reconnecting, disconnectedError);
    addAllowedSubstatesTransitions(reconnecting, disconnectedNormal);
    addAllowedSubstatesTransitions(reconnecting, disconnecting);

    addAllowedSubstatesTransitions(reconnectingError, reconnecting);
    addAllowedSubstatesTransitions(reconnectingError, disconnecting);

    // True=>
    addAllowedSubstatesTransitions(addressReceived, disconnecting);
    addAllowedSubstatesTransitions(addressReceived, reconnecting);

    // FalseToTrue => *
    addAllowedSubstatesTransitions(disconnecting, disconnectedNormal);
    addAllowedSubstatesTransitions(disconnecting, disconnectedError);
}
