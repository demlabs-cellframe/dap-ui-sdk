#include <QtDebug>

#include "DapServiceClient.h"
#include "DapClientDefinitions.h"

/**
 * @brief DapServiceClient::SapServiceClient
 * @param parent
 */
DapServiceClient::DapServiceClient(const QString& a_serviceName,
                                   QObject* parent)
    : QObject(parent)
{
    m_serviceName = a_serviceName;

    sockCtl = new DapUiSocket();

    dapCmdParser = new DapCmdParser(sockCtl, this);

    connect(dapCmdParser, &DapCmdParser::cmdReady, this, &DapServiceClient::procCmdController);

    connect(sockCtl,static_cast<void(DapUiSocket::*)(DapUiSocketError)> (&DapUiSocket::errorOccurred) ,
            this, &DapServiceClient::onCtlSocketError);
    connect(sockCtl,&DapUiSocket::connected, this, &DapServiceClient::onCtlSocketConnected);

    connect(sockCtl,&DapUiSocket::disconnected, this, &DapServiceClient::ctlDisconnected);

    connect(&connectTimer, SIGNAL(timeout()), this, SLOT(connectToService()));
}

/**
 * @brief DapServiceClient::onCtlSocketConnected
 */
void DapServiceClient::onCtlSocketConnected() {
    emit ctlConnected();
    if(this->connectTimer.isActive()) {
        qInfo() << "Connect timer stopped";
        connectTimer.stop();
    }
}

/**
 * @brief DapServiceClient::init
 */
void DapServiceClient::init()
{
    qDebug() << "[DapServiceClient::init]";
    DapServiceNative::init();
    emit serviceInitialized();
}

/**
 * @brief DapServiceClient::connectToService
 */
void DapServiceClient::connectToService()
{
    sockCtl->abort(); // sometimes need to abort previous pending connecting to connect then faster
#ifdef DAP_UI_SOCKET_TCP
    qDebug() << "Connecting to localhost: " << SERVICE_LOCAL_PORT;
    sockCtl->connectToHost(QHostAddress::LocalHost, SERVICE_LOCAL_PORT);
#else
    qDebug() << "Connecting to local server " << DAP_BRAND;
    sockCtl->connectToServer(DAP_BRAND);
#endif
}

/**
 * @brief DapServiceClient::sendCmd
 * @details Send command to the DapVPNService
 * @param a_cmd
 */
void DapServiceClient::sendCmd(const QString & a_cmd)
{
    // TODO: Extended output for problem diagnosis
    // qDebug() << "[DapServiceClient] sock ctl send command: "<< a_cmd << " socket state: " << sockCtl->state();

    if(sockCtl->isWritable())
        sockCtl->write(QString("%1%2").arg(a_cmd).arg('\n').toUtf8());
    else
        qCritical() << "Can't send command. Socket is not writable";
}


/**
 * @brief DapServiceClient::onCtlSocketError
 * @param socketError
 */
void DapServiceClient::onCtlSocketError(DapUiSocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << "onCtlSocketError() sockCtl->errorString() == " <<sockCtl->errorString();

    emit ctlError(sockCtl->errorString());
    if (sockCtl->state() != QAbstractSocket::ConnectedState) {
        startReconnectingToService();
    }
}

void DapServiceClient::startReconnectingToService()
{
    if(!connectTimer.isActive()) {
        qInfo() << "Start trying to reconnect to service";
        connectTimer.start(RECONNECT_TIMEOUT_MS);
    }
}
