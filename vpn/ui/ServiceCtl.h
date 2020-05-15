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
    void setInsurerConnect(bool flag){bInsurerConnect = flag;}
    bool getInsurerConnect(){return bInsurerConnect;}
    void clearServiceRestartCounter(){serviceRestartCounter = 0;}
    int serviceRestartCounter = 0;

protected:
    void procCmdController(const QByteArray &a_cmd) override;
private:
    DapJsonCmdController* m_controller;
    bool bInsurerConnect = false;
};

#endif // SERVICECTL_H
