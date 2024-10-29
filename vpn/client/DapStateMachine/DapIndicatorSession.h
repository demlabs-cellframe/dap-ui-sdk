#ifndef DAPINDICATORSESSION_H
#define DAPINDICATORSESSION_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorSession : public DapIndicatorStateAbstract
{
    Q_OBJECT
public:
    explicit DapIndicatorSession(QObject *parent = nullptr)
        : DapIndicatorStateAbstract(parent) {}

    void init(DapState *s, const QString& stateName) override;
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

    DapState *no_cdbModeConnecting;
    DapState *no_cdbModeConnect;
    DapState *no_cdbDisconnect;

//    DapState *no_cdbMode;
//    DapState *no_cdbModeDisconnecting;
};

#endif // DAPINDICATORSESSION_H
