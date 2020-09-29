#include "DapIndicatorStateAbstract.h"
#include <QDebug>

void DapIndicatorStateAbstract::init(QStateMachine &sm, const QString& stateName)
{
    _parentState = new DapState(stateName, &sm);
    _false = new DapState(_parentState->name() + "False", _parentState);
    _falseToTrue = new DapState(_parentState->name() + "FalseToTrue", _parentState);
    _true = new DapState(_parentState->name() + "True", _parentState);
    _trueToFalse = new DapState(_parentState->name() + "TrueToFalse", _parentState);
    _parentState->setInitialState(_false);

    auto initChangedSignal = [this] (DapState *state, IndicatorState i_state) {
        connect(state, &DapState::entered, [=] {
            emit stateChanged(i_state);
        });
    };

    initChangedSignal(_false, IndicatorState::False);
    initChangedSignal(_falseToTrue, IndicatorState::FalseToTrue);
    initChangedSignal(_true, IndicatorState::True);
    initChangedSignal(_trueToFalse, IndicatorState::TrueToFalse);
}

void DapIndicatorStateAbstract::addAllowedSubstatesTransitions(DapState *from, DapState *to)
{
    _allowedTransitions.insert(from, to);
}

void DapIndicatorStateAbstract::addTransition(DapState *from, DapState *to, const QObject *sender, const char *signal)
{
    if (!isAllowedSubstateTransitions(from, to)){
        qWarning() << "Transition not allowed";
        return;
    }
    from->addTransition(sender, signal, to);
    /*qDebug() << "Added transition from:" << from->name() <<
                "to:" << to->name(); */
}

bool DapIndicatorStateAbstract::isAllowedSubstateTransitions(DapState *from, DapState *to)
{
    QList<DapState*> values = _allowedTransitions.values(from);
    if (values.isEmpty()) {
        qWarning() << from->name() + " state not contain in allowedTransitions map";
        return false;
    }

    for(const auto& value : values) {
        if(value == to) {
            return true;
        }
    }
    qWarning() << "Transition from" << from->name() << "to" << to->name()
               << "not allowed";
    return false;
}
