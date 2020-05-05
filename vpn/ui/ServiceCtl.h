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

    bool startService();
    void setServiceIsOn(bool flag){bServiceIsOn = flag;}
    bool getServiceIsOn(){return bServiceIsOn;}
    void clearServiceRestartCounter(){serviceRestartCounter = 0;}
    int serviceRestartCounter = 0;
protected:
    void procCmdController(const QByteArray &a_cmd) override;
private:
    DapJsonCmdController* m_controller;
    bool bServiceIsOn = false;
};

#endif // SERVICECTL_H
