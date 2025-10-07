#include <QtDebug>
#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif
#include "ServiceCtlOld.h"
#include "dap_common.h"

ServiceCtlOld::ServiceCtlOld(DapCommandControllerAbstract* controller, QObject *parent)
    : DapServiceClient("DAP_SERVICE_NAME", parent), m_controller(controller)
{
    Q_ASSERT(controller);

    connect(m_controller, &DapCommandControllerAbstract::sendDapCmd,
            this, &DapServiceClient::sendCmd);

    connect(this,&ServiceCtlOld::ctlConnected, [=]{
        qInfo() << "[ServiceCtl] Connected to ctl socket,request for status";
    });
    
    connect(this,&ServiceCtlOld::ctlDisconnected, [=]
    {
        qInfo() << "[ServiceCtl] Disconnected from backend";
        bInsurerConnect = true;
        startReconnectingToService();
        startService();
    });
}

bool ServiceCtlOld::startService(){
    qDebug() << "[ServiceCtl] startService()";
    
    int ret = -1; //Bad result on default
    
    //for (int i = 0; i < 2; i++){ // Why should we try to start service twice?
#ifdef Q_OS_WIN
    ret = exec_silent("sc start " DAP_BRAND "Service"); // Should also check there if service is running.
#else
    ret = ::system("systemctl is-active " DAP_BRAND "Service"); // To keep service from restarting twice.
    if (ret != 0) {
        ret = ::system("systemctl start " DAP_BRAND "Service");
    }
    else {
        return false;
    }
#endif
    if (ret == 0) {
        qDebug() << "[ServiceCtl] Start " DAP_BRAND "Service";
        serviceRestartCounter++;
        emit started();
        return true;
    } else {
        qCritical() << "[ServiceCtl] " DAP_BRAND "Service not starting";
    }
   // }
    return false;
}

void ServiceCtlOld::procCmdController(const QByteArray &a_cmd)
{
    m_controller->handleCmd(a_cmd);
}
