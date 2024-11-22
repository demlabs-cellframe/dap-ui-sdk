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
}

void DapCmdStates::_sendUserRequestState()
{
    bool isConnect = false;
    for (const auto &state: _activeStateMachine->getCachedStates())
    {
        if(state.getStringState() != "false")
        {
            isConnect = true;
            break;
        }
    }

    QString stateValue = isConnect ? "Connect" : "Disconnect";
    sendCmdStates("user_request_state", stateValue);
}
