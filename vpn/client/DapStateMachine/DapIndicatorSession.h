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
    DapState *cdbLogouted;
    DapState *cdbLogined;
    DapState *cdbLogoutRequest;
    DapState *cdbKeyActivation;
    DapState *cdbHandshakeRequest;
    DapState *cdbHandshakeReply;
    DapState *networkErr;
};

#endif // DAPINDICATORSESSION_H
