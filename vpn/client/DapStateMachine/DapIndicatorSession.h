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
    DapState * logouted;
    DapState * authRequestError;
    DapState * streamCtlError;
    DapState * handshakeError;
    DapState * logoutError;
    DapState * networkError;
    DapState * networkErrorIdle;
    DapState * handshakeRequestCanceling;
    DapState * authRequestCanceling;
    DapState * authCanceling;
    DapState * streamRequestCanceling;

    // Substates falseToTrue
    DapState * handshakeRequested;
    DapState * handshakeResponse;
    DapState * authRequested;
    DapState * keyActivationRequested;
    DapState * keyActivated;
    DapState * authorized;
    DapState * streamCtlRequested;

    // Substates True
    DapState * streamSessionOpened;

    // Substates TrueToFalse
    DapState * logoutRequested;
    DapState * unAuthorized;
    //DapState * switchRequested;
};

#endif // DAPINDICATORSESSION_H
