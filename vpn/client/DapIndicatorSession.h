#ifndef DAPINDICATORSESSION_H
#define DAPINDICATORSESSION_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorSession : public DapIndicatorStateAbstract
{
    Q_OBJECT
public:
    explicit DapIndicatorSession(QObject *parent = nullptr)
        : DapIndicatorStateAbstract(parent) {}

    void init(QStateMachine &sm, const QString& stateName) override;
protected:
    void initAllowedSubstatesTransitions() override;
public:
    // Substates false
    DapState *logouted;
    DapState *authError;
    DapState *streamCtlError;
    DapState *handshakeError;
    DapState *logoutError;
    DapState *networkError;

    // Substates falseToTrue
    DapState *handshakeRequested;
    DapState *handshakeResponse;
    DapState *authRequested;
    DapState *authorized;
    DapState *streamCtlRequested;

    // Substates True
    DapState *streamSessionOpened;

    // Substates TrueToFalse
    DapState *logoutRequested;
};

#endif // DAPINDICATORSESSION_H
