#ifndef DapCmdStatesHANDLER_H
#define DapCmdStatesHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapStateMachine.h"

class DapCmdStatesSrv: public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    void _sendUserRequestState();
    DapStateMachine *_activeStateMachine;
public:
    explicit DapCmdStatesSrv(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    void sendCmdStates(const QString& stateName, const QString stateVal);
};

#endif // DapCmdStatesHANDLER_H
