#include "DapCmdStatesSrv.h"

void DapCmdStatesSrv::handle(const QJsonObject* params) {
    Q_UNUSED(params);

    for (const auto &di: _activeStateMachine->getCachedStates()) {
        sendCmdStates(di.getStringType(), di.getStringState());
    }

    _sendUserRequestState();
}

void DapCmdStatesSrv::sendCmdStates(const QString& stateName, const QString stateVal)
{
    QJsonObject response;
    response["state_name"] = stateName;
    response["state"] = stateVal;
    sendCmd(&response);
}

DapCmdStatesSrv::DapCmdStatesSrv(QObject *parent)
    : DapCmdServiceAbstract(DapJsonCmdType::GET_STATES, parent)
{
    _activeStateMachine = DapStateMachine::instance();
}

void DapCmdStatesSrv::_sendUserRequestState()
{
    QString stateValue = _activeStateMachine->userRequestStateConnect->active() ?
                "Connect" : "Disconnect";
    sendCmdStates("user_request_state", stateValue);
}
