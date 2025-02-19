#include "DapIndicatorStateAbstract.h"
#include <QDebug>
#include <QSignalTransition>

void DapIndicatorStateAbstract::init(DapState *s, const QString& stateName)
{
    _parentState = new DapState(stateName, s);
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

void DapIndicatorStateAbstract::logTransition()
{
    QSignalTransition* transition = qobject_cast<QSignalTransition*>(sender());
    if (transition) {
        QAbstractState* fromState = transition->sourceState();
        QAbstractState* toState = transition->targetState();

        DapState* fromDapState = dynamic_cast<DapState*>(fromState);
        DapState* toDapState = dynamic_cast<DapState*>(toState);

        QString fromStateName = fromDapState ? fromDapState->name() : "Unknown";
        QString toStateName = toDapState ? toDapState->name() : "Unknown";

        qDebug() << "[DapIndicatorStateAbstract] From: " << fromStateName << " to: " << toStateName;
    } else {
        qDebug() << "Error";
    }
}

void DapIndicatorStateAbstract::addTransition(DapState *from, DapState *to, const QObject *sender, const char *signal)
{
    if (!isAllowedSubstateTransitions(from, to)){
        qWarning() << "Transition not allowed. From: " << from->objectName() << " to: " << to->objectName();
        return;
    }
    QSignalTransition* transition = from->addTransition(sender, signal, to);
    if (transition) {
        connect(transition, &QSignalTransition::triggered, this, &DapIndicatorStateAbstract::logTransition);
    }
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
