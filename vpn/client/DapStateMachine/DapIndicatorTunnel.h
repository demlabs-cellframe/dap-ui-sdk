#ifndef DAPINDICATORTUNNEL_H
#define DAPINDICATORTUNNEL_H

#include "DapIndicatorStateAbstract.h"

class DapIndicatorTunnel: public DapIndicatorStateAbstract
{
    Q_OBJECT
public:
    explicit DapIndicatorTunnel(QObject *parent = nullptr)
        : DapIndicatorStateAbstract(parent) { }
    void init(DapState *s, const QString& stateName) override;
protected:
    void initAllowedSubstatesTransitions() override;
public:
    DapState *tunDisabled;
    DapState *tunEnabled;
    DapState *tunIdle;
};

#endif // DAPINDICATORTUNNEL_H
