#include <QDebug>
#include "DapState.h"


DapState::DapState(const QString& stateName,
                   QState *parent,
                   bool isErrorState)
    : QState(parent)
{
    _name = stateName;
    _isErrorState = isErrorState;

    connect(this, &QState::entered, [=] {
        if(!_isErrorState)
            qInfo() << "Entered in state" << _name;
        else
            qWarning() << "Entered in state" << _name;
    });
}

DapState::~DapState()
{

}
