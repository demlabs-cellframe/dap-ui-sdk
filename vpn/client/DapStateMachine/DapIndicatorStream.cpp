#include "DapIndicatorStream.h"

void DapIndicatorStream::init(QStateMachine &sm, const QString& stateName)
{
    DapIndicatorStateAbstract::init(sm, stateName);

    upsDisconnected     = new DapState(_false->name()       + "_disconnected",      _false);
    upsHandshakeRequest = new DapState(_falseToTrue->name() + "_handshakeRequest",  _falseToTrue);
    upsHandshakeReply   = new DapState(_falseToTrue->name() + "_handshakeReply",    _falseToTrue);
    upsChannelsOpen     = new DapState(_falseToTrue->name() + "_channelsOpen",      _falseToTrue);
    networkErr          = new DapState(_false->name()       + "_networkError",      _false, true);
    srvProvided         = new DapState(_falseToTrue->name() + "_serviceProvided",   _falseToTrue);
    configProvided      = new DapState(_true->name()        + "_netconfigProvided", _true);

    // Init substates false
    /*disconnectedNormal = new DapState(_false->name() + "DisconnectedNormal", _false);
    disconnectedError  = new DapState(_false->name() + "DisconnectedError", _false, true);
    ipRequestError     = new DapState(_false->name() + "RequestError", _false, true);
    serviceError       = new DapState(_false->name() + "ServiceError", _false, true);*/

    _false->setInitialState(upsDisconnected);

    // Init substates falseToTrue
    /*connected          = new DapState(_falseToTrue->name() + "Connected",
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
                                            _trueToFalse);*/
    initAllowedSubstatesTransitions();
}

void DapIndicatorStream::initAllowedSubstatesTransitions()
{
    addAllowedSubstatesTransitions(upsDisconnected,         upsHandshakeRequest);
    addAllowedSubstatesTransitions(upsHandshakeRequest,     upsHandshakeReply);
    addAllowedSubstatesTransitions(upsHandshakeRequest,     networkErr);
    addAllowedSubstatesTransitions(upsHandshakeRequest,     upsDisconnected);
    addAllowedSubstatesTransitions(upsHandshakeReply,       upsChannelsOpen);
    addAllowedSubstatesTransitions(upsHandshakeReply,       networkErr);
    addAllowedSubstatesTransitions(upsHandshakeReply,       upsDisconnected);
    addAllowedSubstatesTransitions(upsChannelsOpen,         srvProvided);
    addAllowedSubstatesTransitions(upsChannelsOpen,         upsDisconnected);
    addAllowedSubstatesTransitions(upsChannelsOpen,         networkErr);
    addAllowedSubstatesTransitions(srvProvided,             configProvided);
    addAllowedSubstatesTransitions(srvProvided,             networkErr);
    addAllowedSubstatesTransitions(srvProvided,             upsDisconnected);
    addAllowedSubstatesTransitions(configProvided,          networkErr);
    addAllowedSubstatesTransitions(configProvided,          upsDisconnected);
    addAllowedSubstatesTransitions(networkErr,              upsHandshakeRequest);
    addAllowedSubstatesTransitions(networkErr,              upsDisconnected);
    // False
    /*addAllowedSubstatesTransitions(disconnectedNormal, connected);
    addAllowedSubstatesTransitions(disconnectedNormal, reconnectingError);
    addAllowedSubstatesTransitions(disconnectedError, connected);
    addAllowedSubstatesTransitions(ipRequestError, connected);

    // FalseToTrue => *
    //addAllowedSubstatesTransitions(connected, ipRequested);
    addAllowedSubstatesTransitions(connected, serviceRequested);
    addAllowedSubstatesTransitions(serviceRequested, serviceSuccess);
    addAllowedSubstatesTransitions(serviceRequested, serviceError);
    addAllowedSubstatesTransitions(serviceRequested, reconnecting);
    addAllowedSubstatesTransitions(serviceRequested, disconnecting);
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
    addAllowedSubstatesTransitions(disconnecting, disconnectedError);*/
}
