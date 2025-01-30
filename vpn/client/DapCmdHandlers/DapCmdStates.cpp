#include "DapCmdStates.h"

void DapCmdStates::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    QString userRequestState = getUserRequestState();
    QJsonObject result;

    result.insert("states", _activeStateMachine->getJsonCachedStates());
    if(!userRequestState.isEmpty()){
        result.insert(QStringLiteral("user_request_state"), userRequestState);
        qDebug() << "[DapCmdStates] type:  user_request_state, state: " << userRequestState;
    }
    sendCmd(&result);
}

void DapCmdStates::sendServerChanged()
{
    QJsonObject result;
    result.insert("server_change_state", "ServerChanged");
    sendCmd(&result);
}

void DapCmdStates::sendReconnectingState()
{
    QJsonObject response;
    const QString stateName = "reconnecting_state";
    const QString stateVal = "Reconnecting";

    qDebug() << "StateInfo\t state_name: " << stateName << "\tstate: " << stateVal;
    response["state_name"] = stateName;
    response["state"] = stateVal;
    sendCmd(&response);
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
