#ifndef DAPINDICATORSTREAM_H
#define DAPINDICATORSTREAM_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorStream : public DapIndicatorStateAbstract {
    Q_OBJECT
public:
    explicit DapIndicatorStream(QObject *parent = nullptr): DapIndicatorStateAbstract(parent) { }
    void init(QStateMachine &sm, const QString& stateName) override;
protected:
    void initAllowedSubstatesTransitions() override;
public:
    static const time_t STREAM_RECONNECTING_TIMEOUT = 1500;

    DapState *upsDisconnected;
    DapState *upsHandshakeRequest;
    DapState *upsHandshakeReply;
    DapState *upsChannelsOpen;
    DapState *networkErr;
    DapState *srvProvided;
    DapState *configProvided;

    // Substates False
    /*DapState* disconnectedNormal;
    DapState* disconnectedError;

    // Substates for chain net services
    DapState* serviceRequested;
    DapState* serviceError;
    DapState* serviceSuccess;


    DapState* ipRequestError;
    // Substates falseToTrue
    DapState* ipRequested;
    DapState* connected;
    DapState* connectError;
    DapState* reconnecting;
    DapState* reconnectingError;

    // Substates true
    DapState* addressReceived;

    // Substates TrueToFalse
    DapState* disconnecting;*/
};

#endif // DAPINDICATORSTREAM_H
