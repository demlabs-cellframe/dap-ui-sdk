#ifndef DAPCMDSTATESHANDLER_H
#define DAPCMDSTATESHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapStateMachine.h"

class DapCmdStates: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdStates(QObject *parent = nullptr);
    void handle(const QJsonObject* statesInfo) override;
    void sendServerChanged();
    void sendReconnectingState();
private:
    void sendCmdStates(const QString& stateName, const QString stateVal);
    void sendUserRequestState();
    QString getUserRequestState();
private:
    DapStateMachine *_activeStateMachine;
};

#endif // DAPCMDSTATESHANDLER_H
