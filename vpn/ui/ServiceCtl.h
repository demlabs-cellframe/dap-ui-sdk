#ifndef SERVICECTL_H
#define SERVICECTL_H

#include "DapServiceClient.h"
#include <QMap>
#include <DapJsonCmdController.h>

class ServiceCtl;

class QTimer;
class ServiceCtl : public DapServiceClient
{
    Q_OBJECT
public:
    ServiceCtl(DapJsonCmdController* controller, QObject *parent = Q_NULLPTR);
    bool tapStatus = false;
protected:
    void procCmdController(const QByteArray &a_cmd) override;
private:
    DapJsonCmdController* m_controller;
    QTimer * tmRestart;
};

#endif // SERVICECTL_H
