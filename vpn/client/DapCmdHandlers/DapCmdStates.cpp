#include "DapCmdStates.h"

void DapCmdStates::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);

    for (const auto &di: _activeStateMachine->getCachedStates()) {
        sendCmdStates(di.getStringType(), di.getStringState());
    }

    _sendUserRequestState();
}

void DapCmdStates::sendCmdStates(const QString& stateName, const QString stateVal)
{
    QJsonObject response;
    qDebug() << "StateInfo\t state_name: " << stateName << "\tstate: " << stateVal;
    response["state_name"] = stateName;
    response["state"] = stateVal;
    sendCmd(&response);
}

DapCmdStates::DapCmdStates(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_STATES, parent)
{
    _activeStateMachine = DapStateMachine::instance();
    Q_ASSERT(_activeStateMachine);
}

void DapCmdStates::_sendUserRequestState()
{
    const QVector<DapIndicator>& states = _activeStateMachine->getCachedStates();

    if (states.empty()) {
        sendCmdStates(QStringLiteral("user_request_state"), QStringLiteral("Disconnect"));
        return;
    }

    bool allFalse = std::all_of(states.begin(), states.end(), [](const DapIndicator& state) {
        return state.getStringState() == QStringLiteral("false");
    });

    bool allTrue = std::all_of(states.begin(), states.end(), [](const DapIndicator& state) {
        return state.getStringState() == QStringLiteral("true");
    });

    if (allFalse) {
        sendCmdStates(QStringLiteral("user_request_state"), QStringLiteral("Disconnect"));
    } else if (allTrue) {
        sendCmdStates(QStringLiteral("user_request_state"), QStringLiteral("Connect"));
    }
}
