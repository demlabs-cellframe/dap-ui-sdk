#ifndef DAPSTATE_H
#define DAPSTATE_H

#include <QObject>
#include <QState>
#include <QList>

class DapState : public QState
{
    Q_OBJECT
public:
    /* The second parameter is a slot of object.
     * He will be called when state has been changed
     * and if this action will be for this state */
    using StateAction = QPair<QObject*, const char*>;

    explicit DapState(const QString& stateName,
                      QState *parent,
                      bool isErrorState = false);

    virtual ~DapState();

    void init(QState *parent);

    bool isErrorState() { return _isErrorState;}
    const QString& name() { return _name; }
protected:
    // Only friend class can add transition
    friend class DapIndicatorStateAbstract;
    friend class DapStateMachine;
    QSignalTransition *addTransition(const QObject *sender, const char *signal, QAbstractState *target)
    { return QState::addTransition(sender, signal, target); }
    void addTransition(QAbstractTransition *transition) { QState::addTransition(transition); }
    QAbstractTransition *addTransition(QAbstractState *target) { return QState::addTransition(target); }

private:
    bool _isErrorState;
    QString _name;
signals:
public slots:
};

#endif // DAPSTATE_H
