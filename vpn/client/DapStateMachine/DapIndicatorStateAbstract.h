#ifndef DAPINDICATORSTATEABSTRACT_H
#define DAPINDICATORSTATEABSTRACT_H

#include <QObject>
#include <QStateMachine>
#include <QMultiMap>
#include "DapState.h"
#include "DapIndicator.h"

class DapIndicatorStateAbstract : public QObject
{
    Q_OBJECT
public:
    using IndicatorState = DapIndicator::State;

    explicit DapIndicatorStateAbstract(QObject *parent = nullptr)
        : QObject(parent) {}

    virtual void init(DapState *s, const QString& stateName);
    bool isAllowedSubstateTransitions(DapState *from, DapState *to);

    void addTransition(DapState *from, DapState *to, const QObject *sender, const char *signal);

    virtual ~DapIndicatorStateAbstract() {}
protected:
    DapState *_parentState;
    DapState *_false;
    DapState *_falseToTrue;
    DapState *_true;
    DapState *_trueToFalse;

signals:
    // Emit when entered in some indicator state
    void stateChanged(IndicatorState newState);
protected:
    void addAllowedSubstatesTransitions(DapState *from, DapState *to);
    virtual void initAllowedSubstatesTransitions() = 0;
private:
    QMultiMap<DapState*, DapState*> _allowedTransitions;
};

#endif // DAPINDICATORSTATEABSTRACT_H
