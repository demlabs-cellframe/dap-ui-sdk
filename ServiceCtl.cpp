#include <QtDebug>
#include <QSettings>
#include <QTimer>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#endif
#include "ServiceCtl.h"


ServiceCtl::ServiceCtl(DapJsonCmdController* controller, QObject *parent)
    : DapServiceClient("DAP_SERVICE_NAME", parent), m_controller(controller)
{
    Q_ASSERT(controller);
    tmRestart = new QTimer(this);

    connect(m_controller, &DapJsonCmdController::sendDapCmd,
            this, &DapServiceClient::sendCmd);

    connect(this,&ServiceCtl::ctlConnected, [=]{
        qInfo() << "[ServiceCtl] Connected to ctl socket,request for status";
        tmRestart->stop();
    });
    
    connect(this,&ServiceCtl::ctlDisconnected, [=]{
        qInfo() << "[ServiceCtl] Disconnected from backend";
        tmRestart->start(2000);
        startReconnectingToService();
    });

    connect(tmRestart, &QTimer::timeout, [=]{ restartService(); });
}

void ServiceCtl::procCmdController(const QByteArray &a_cmd)
{
    m_controller->handleCmd(a_cmd);
}
