#ifndef SERVICECTLOLD_H
#define SERVICECTLOLD_H

#include "DapServiceClient.h"
#include <QMap>
#include <DapCommandControllerAbstract.h>

class ServiceCtlOld;

class QTimer;
class ServiceCtlOld : public DapServiceClient
{
    Q_OBJECT
public:
    ServiceCtlOld(DapCommandControllerAbstract* controller, QObject *parent = Q_NULLPTR);
    bool tapStatus = false;

    bool startService();
    void setInsurerConnect(bool flag){bInsurerConnect = flag;}
    bool getInsurerConnect(){return bInsurerConnect;}
    void clearServiceRestartCounter(){serviceRestartCounter = 0;}
    int serviceRestartCounter = 0;

protected:
    void procCmdController(const QByteArray &a_cmd) override;
signals:
    void started();
private:
    DapCommandControllerAbstract* m_controller;
    bool bInsurerConnect = false;
};

#endif // SERVICECTLOLD_H
