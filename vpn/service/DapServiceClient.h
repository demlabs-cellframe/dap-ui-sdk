#ifndef DAPSERVICECLIENT_H
#define DAPSERVICECLIENT_H

#include <QObject>
#include <QTimer>

#define DAP_UI_SOCKET_TCP

#ifdef DAP_UI_SOCKET_TCP
#include <QTcpSocket>
#include <QHostAddress>
typedef QTcpSocket DapUiSocket;
typedef QTcpSocket::SocketError DapUiSocketError;
#else
#include <QLocalSocket>
typedef QLocalSocket DapUiSocket;
typedef QLocalSocket::LocalSocketError DapUiSocketError;
#endif

#ifdef Q_OS_ANDROID
#include "DapAndroidCompat.h"
#endif

#if defined(Q_OS_ANDROID)
#include "DapServiceNativeAndroid.h"
typedef class DapServiceNativeAndroid DapServiceNative;
#elif defined(Q_OS_LINUX)
#include "DapServiceNativeLinux.h"
typedef class DapServiceNativeLinux DapServiceNative;
#elif defined(Q_OS_DARWIN)
#include "DapServiceNativeDarwin.h"
typedef class DapServiceNativeDarwin DapServiceNative;
#elif defined(Q_OS_WIN)
#include "DapServiceNativeWindows.h"
typedef class DapServiceNativeWindows DapServiceNative;
#elif defined(Q_OS_IOS)
#include "DapServiceNativeIOS.h"
typedef class DapServiceNativeIOS DapServiceNative;
#endif

#include "DapCmdParser.h"

class DapServiceClient : public QObject, public DapServiceNative
{
    Q_OBJECT
public:
    const QString& serviceName(){ return m_serviceName; }
    bool getIsSockConnected(){ return sockCtl->state() == QTcpSocket::ConnectedState; }
signals:
    void ctlConnected();
    void ctlDisconnected();
    void ctlError(QString);
    void sigServiceConnectionFailed();

    void error(QString);

    void serviceInitialized();

    void sigTunTapError();
    void sigTunTapPresent();
public slots:
    void sendCmd(const QString & a_cmd);
    void init() override;
    void connectToService();
    void startReconnectingToService();
    void resetReconnectState();

protected:

    DapServiceClient(const QString& a_serviceName, QObject* parent = Q_NULLPTR);
protected slots:
    virtual void procCmdController(const QByteArray &a_cmd) = 0;
private:
    static constexpr int RECONNECT_INITIAL_MS = 1000;
    static constexpr int RECONNECT_MAX_MS     = 30000;
    static constexpr int RECONNECT_MAX_ATTEMPTS = 15;

    QTimer connectTimer;
    DapUiSocket * sockCtl;
    QString m_serviceName;
    QByteArray readBuffer;
    DapCmdParser * dapCmdParser;
    int m_reconnectAttempts  = 0;
    int m_currentBackoffMs   = RECONNECT_INITIAL_MS;

private slots:
    void onCtlSocketConnected();
    void onCtlSocketError(DapUiSocketError socketError);
};
#endif // DAPSERVICECLIENT_H
