#include "DapCmdStates.h"

void DapCmdStates::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);

    QString userRequestState = getUserRequestState();

    QJsonObject result;
    QJsonObject statesInfo;
    for (const auto &state: _activeStateMachine->getCachedStates()) {
        statesInfo.insert(state.getStringType(), state.getStringState());
    }
    result.insert("states", statesInfo);
    if(!userRequestState.isEmpty()){
        result.insert(QStringLiteral("user_request_state"), userRequestState);
    }
    sendCmd(&result);
}

void DapCmdStates::sendServerChanged()
{
    QJsonObject result;
    result.insert("server_change_state", "ServerChanged");
    sendCmd(&result);
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

QString DapCmdStates::getUserRequestState()
{
    const QVector<DapIndicator>& states = _activeStateMachine->getCachedStates();

    if (states.empty()) {
        return QStringLiteral("Disconnect");
    }

    bool allFalse = std::all_of(states.begin(), states.end(), [](const DapIndicator& state) {
        return state.getStringState() == QStringLiteral("false");
    });

    bool allTrue = std::all_of(states.begin(), states.end(), [](const DapIndicator& state) {
        return state.getStringState() == QStringLiteral("true");
    });

    if (allFalse) {
        return QStringLiteral("Disconnect");
    }
    else if (allTrue) {
        return QStringLiteral("Connect");
    }

    return QString();
}
