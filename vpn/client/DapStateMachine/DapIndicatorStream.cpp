#include "DapIndicatorStream.h"

void DapIndicatorStream::init(DapState *s, const QString& stateName)
{
    DapIndicatorStateAbstract::init(s, stateName);
    upsDisconnected     = new DapState(_false->name()       + "_disconnected",      _false);
    upsHandshakeRequest = new DapState(_falseToTrue->name() + "_handshakeRequest",  _falseToTrue);
    upsHandshakeReply   = new DapState(_falseToTrue->name() + "_handshakeReply",    _falseToTrue);
    upsChannelsOpen     = new DapState(_falseToTrue->name() + "_channelsOpen",      _falseToTrue);
    networkErr          = new DapState(_false->name()       + "_networkError",      _false, true);
    srvProvided         = new DapState(_falseToTrue->name() + "_serviceProvided",   _falseToTrue);
    configProvided      = new DapState(_true->name()        + "_netconfigProvided", _true);
    streamReconnecting  = new DapState(_false->name()       + "_reconnecting",      _false);

    _false->setInitialState(upsDisconnected);

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
    addAllowedSubstatesTransitions(configProvided,          upsHandshakeRequest);
    addAllowedSubstatesTransitions(networkErr,              upsHandshakeRequest);
    addAllowedSubstatesTransitions(networkErr,              upsDisconnected);
    addAllowedSubstatesTransitions(networkErr,              streamReconnecting);
    addAllowedSubstatesTransitions(streamReconnecting,      upsDisconnected);
    addAllowedSubstatesTransitions(streamReconnecting,      upsChannelsOpen);
    addAllowedSubstatesTransitions(streamReconnecting,      configProvided);
}
