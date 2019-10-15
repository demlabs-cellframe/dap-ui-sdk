#ifndef DAPCMDSTATESHANDLER_H
#define DAPCMDSTATESHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapStateMachine.h"

class DapCmdStates: public DapCmdServiceAbstract
{
    Q_OBJECT
private:
    void _sendUserRequestState();
    DapStateMachine *_activeStateMachine;
public:
    explicit DapCmdStates(QObject *parent = nullptr);
    void handle(const QJsonObject* params) override;
    void sendCmdStates(const QString& stateName, const QString stateVal);
};

#endif // DAPCMDSTATESHANDLER_H
