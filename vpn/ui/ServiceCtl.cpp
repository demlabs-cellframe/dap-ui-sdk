#include <QtDebug>
#include <QSettings>
#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif
#include "ServiceCtl.h"
#include "dap_common.h"

ServiceCtl::ServiceCtl(DapJsonCmdController* controller, QObject *parent)
    : DapServiceClient("DAP_SERVICE_NAME", parent), m_controller(controller)
{
    Q_ASSERT(controller);

    connect(m_controller, &DapJsonCmdController::sendDapCmd,
            this, &DapServiceClient::sendCmd);

    connect(this,&ServiceCtl::ctlConnected, [=]{
        qInfo() << "[ServiceCtl] Connected to ctl socket,request for status";
        bServiceIsOn = true;
    });
    
    connect(this,&ServiceCtl::ctlDisconnected, [=]{

        qInfo() << "[ServiceCtl] Disconnected from backend";
        bServiceIsOn = false;
        startReconnectingToService();
        startService();
    });
}

bool ServiceCtl::startService(){
    qDebug() << "[ServiceCtl] startService()";
    
    int ret = -1; //Bad result on default
    
    for (int i = 0; i < 2; i++){ // Why should we try to start service twice?
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
        if (ret != 0) {
            qDebug() << "[ServiceCtl] Start " DAP_BRAND "Service";
            bServiceIsOn = true;
            serviceRestartCounter++;
            return true;
        } else {
            qCritical() << "[ServiceCtl] " DAP_BRAND "Service not starting";
        }
    }
    return false;
}

void ServiceCtl::procCmdController(const QByteArray &a_cmd)
{
    m_controller->handleCmd(a_cmd);
}
