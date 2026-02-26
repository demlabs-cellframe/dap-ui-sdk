#ifndef DAPCMDSTATESHANDLER_H
#define DAPCMDSTATESHANDLER_H

#include <QObject>
#include "DapCmdServiceAbstract.h"
#include "DapStateMachine.h"

class DapStateConnectionController;

class DapCmdStates: public DapCmdServiceAbstract
{
    Q_OBJECT
public:
    explicit DapCmdStates(QObject *parent = nullptr);
    void handle(const QJsonObject* statesInfo) override;
    void sendServerChanged();
    void setConnectionController(DapStateConnectionController *controller);
private:
    void sendCmdStates(const QString& stateName, const QString stateVal);
    void sendUserRequestState();
    QString getUserRequestState();
    QString getConnectionStateName() const;
private:
    DapStateMachine *_activeStateMachine;
    DapStateConnectionController *m_connectionController = nullptr;
};

#endif // DAPCMDSTATESHANDLER_H
