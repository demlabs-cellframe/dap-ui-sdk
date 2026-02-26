#include "DapCmdStates.h"
#include "DapStateConnectionController.h"

void DapCmdStates::handle(const QJsonObject* params)
{
    DapCmdServiceAbstract::handle(params);
    QString userRequestState = getUserRequestState();
    QJsonObject result;

    result.insert("states", _activeStateMachine->getJsonCachedStates());
    result.insert(QStringLiteral("tunnel_active"), _activeStateMachine->isTunnelActive());
    result.insert(QStringLiteral("connection_state"), getConnectionStateName());
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

void DapCmdStates::setConnectionController(DapStateConnectionController *controller)
{
    m_connectionController = controller;
}

QString DapCmdStates::getConnectionStateName() const
{
    if(!m_connectionController)
        return QStringLiteral("DISCONNECT");

    switch(m_connectionController->getCurrentState())
    {
    case DapStateConnectionController::DISCONNECT:    return QStringLiteral("DISCONNECT");
    case DapStateConnectionController::CONNECTING:    return QStringLiteral("CONNECTING");
    case DapStateConnectionController::CONNECT:       return QStringLiteral("CONNECT");
    case DapStateConnectionController::DISCONNECTING: return QStringLiteral("DISCONNECTING");
    }
    return QStringLiteral("DISCONNECT");
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
