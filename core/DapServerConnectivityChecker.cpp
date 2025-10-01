#include "DapServerConnectivityChecker.h"
#include "DapHttpPing.h"
#include <QTcpSocket>
#include <QHostInfo>
#include <QTimer>
#include <QDebug>

DapServerConnectivityChecker::DapServerConnectivityChecker(QObject *parent)
    : QObject(parent)
    , m_port(0)
    , m_timeoutMs(2000)
    , m_debugMode(false)
    , m_currentMethod(DapConnectionMethod::TCP_CONNECT)
    , m_tcpSocket(nullptr)
    , m_tcpTimer(nullptr)
    , m_dnsLookupId(-1)
    , m_httpPing(nullptr)
    , m_socketPingSocket(nullptr)
    , m_socketPingTimer(nullptr)
{
}

DapServerConnectivityChecker::~DapServerConnectivityChecker()
{
    cleanup();
}

void DapServerConnectivityChecker::checkServerAvailability(const QString& host, quint16 port, int timeoutMs)
{
    m_host = host;
    m_port = port;
    m_timeoutMs = timeoutMs;
    m_currentMethod = DapConnectionMethod::TCP_CONNECT;
    
    debugLog(QString("Starting multi-level connectivity check for %1:%2").arg(host).arg(port));
    
    cleanup();
    m_timer.start();
    
    // Start with TCP connect test
    if (!tcpConnectTest(host, port)) {
        tryNextMethod();
    }
}

bool DapServerConnectivityChecker::tcpConnectTest(const QString& host, quint16 port)
{
    debugLog("Starting TCP connect test");
    
    m_tcpSocket = new QTcpSocket(this);
    m_tcpTimer = new QTimer(this);
    m_tcpTimer->setSingleShot(true);
    m_tcpTimer->setInterval(m_timeoutMs);
    
    connect(m_tcpSocket, &QTcpSocket::connected, this, &DapServerConnectivityChecker::onTcpConnected);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &DapServerConnectivityChecker::onTcpError);
    connect(m_tcpTimer, &QTimer::timeout, this, &DapServerConnectivityChecker::onTcpTimeout);
    
    m_tcpTimer->start();
    m_tcpSocket->connectToHost(host, port);
    
    return true;
}

bool DapServerConnectivityChecker::dnsResolutionTest(const QString& host)
{
    debugLog("Starting DNS resolution test");
    
    m_dnsLookupId = QHostInfo::lookupHost(host, this, 
                                         SLOT(onDnsResolved(QHostInfo)));
    
    // Set timeout for DNS resolution
    QTimer::singleShot(m_timeoutMs, this, [this]() {
        if (m_dnsLookupId != -1) {
            QHostInfo::abortHostLookup(m_dnsLookupId);
            m_dnsLookupId = -1;
            debugLog("DNS resolution timeout");
            tryNextMethod();
        }
    });
    
    return m_dnsLookupId != -1;
}

bool DapServerConnectivityChecker::httpPingTest(const QString& host, quint16 port)
{
    debugLog("Starting HTTP ping test");
    
    if (m_httpPing) {
        m_httpPing->deleteLater();
    }
    
    m_httpPing = new DapHttpPing(host, port);
    m_httpPing->setParent(this);
    m_httpPing->setUseMultiLevelCheck(false); // Disable multi-level check to avoid recursion
    
    connect(m_httpPing, &DapHttpPing::sigResponse, this, &DapServerConnectivityChecker::onHttpPingSuccess);
    connect(m_httpPing, &DapHttpPing::sigNetworkError, this, &DapServerConnectivityChecker::onHttpPingError);
    
    // Set timeout for HTTP ping
    QTimer::singleShot(m_timeoutMs, this, [this]() {
        if (m_httpPing) {
            debugLog("HTTP ping timeout");
            tryNextMethod();
        }
    });
    
    m_httpPing->sendRequest(host, port);
    
    return true;
}

bool DapServerConnectivityChecker::socketPingTest(const QString& host, quint16 port)
{
    debugLog("Starting socket ping test");
    
    m_socketPingSocket = new QTcpSocket(this);
    m_socketPingTimer = new QTimer(this);
    m_socketPingTimer->setSingleShot(true);
    m_socketPingTimer->setInterval(m_timeoutMs);
    
    connect(m_socketPingSocket, &QTcpSocket::connected, this, [this]() {
        m_socketPingTimer->stop();
        int responseTime = m_timer.elapsed();
        debugLog(QString("Socket ping successful, response time: %1ms").arg(responseTime));
        
        DapConnectivityResult result(true, responseTime, DapConnectionMethod::SOCKET_PING);
        completeCheck(result);
    });
    
    connect(m_socketPingSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, [this](QAbstractSocket::SocketError error) {
        Q_UNUSED(error)
        m_socketPingTimer->stop();
        debugLog("Socket ping failed");
        
        DapConnectivityResult result(false, -1, DapConnectionMethod::SOCKET_PING, 
                                   "Socket ping connection failed");
        completeCheck(result);
    });
    
    connect(m_socketPingTimer, &QTimer::timeout, this, [this]() {
        debugLog("Socket ping timeout");
        if (m_socketPingSocket) {
            m_socketPingSocket->abort();
        }
        
        DapConnectivityResult result(false, -1, DapConnectionMethod::SOCKET_PING, "Socket ping timeout");
        completeCheck(result);
    });
    
    m_socketPingTimer->start();
    m_socketPingSocket->connectToHost(host, port);
    
    return true;
}

void DapServerConnectivityChecker::onTcpConnected()
{
    if (m_tcpTimer) {
        m_tcpTimer->stop();
    }
    
    int responseTime = m_timer.elapsed();
    debugLog(QString("TCP connect successful, response time: %1ms").arg(responseTime));
    
    DapConnectivityResult result(true, responseTime, DapConnectionMethod::TCP_CONNECT);
    completeCheck(result);
}

void DapServerConnectivityChecker::onTcpError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    
    if (m_tcpTimer) {
        m_tcpTimer->stop();
    }
    
    debugLog("TCP connect failed, trying next method");
    tryNextMethod();
}

void DapServerConnectivityChecker::onTcpTimeout()
{
    debugLog("TCP connect timeout, trying next method");
    
    if (m_tcpSocket) {
        m_tcpSocket->abort();
    }
    
    tryNextMethod();
}

void DapServerConnectivityChecker::onDnsResolved(const QHostInfo& hostInfo)
{
    m_dnsLookupId = -1;
    
    if (hostInfo.error() == QHostInfo::NoError && !hostInfo.addresses().isEmpty()) {
        int responseTime = m_timer.elapsed();
        debugLog(QString("DNS resolution successful, response time: %1ms").arg(responseTime));
        
        DapConnectivityResult result(true, responseTime, DapConnectionMethod::DNS_RESOLUTION);
        completeCheck(result);
    } else {
        debugLog("DNS resolution failed, trying next method");
        tryNextMethod();
    }
}

void DapServerConnectivityChecker::onHttpPingSuccess(qint64 responseTime)
{
    debugLog(QString("HTTP ping successful, response time: %1ms").arg(responseTime));
    
    DapConnectivityResult result(true, static_cast<int>(responseTime), DapConnectionMethod::HTTP_PING);
    completeCheck(result);
}

void DapServerConnectivityChecker::onHttpPingError(const QString errorMessage)
{
    debugLog(QString("HTTP ping failed: %1, trying next method").arg(errorMessage));
    tryNextMethod();
}

void DapServerConnectivityChecker::tryNextMethod()
{
    // Clean up current method resources
    if (m_tcpSocket) {
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }
    if (m_tcpTimer) {
        m_tcpTimer->deleteLater();
        m_tcpTimer = nullptr;
    }
    
    // Try next method in sequence
    switch (m_currentMethod) {
        case DapConnectionMethod::TCP_CONNECT:
            m_currentMethod = DapConnectionMethod::DNS_RESOLUTION;
            if (!dnsResolutionTest(m_host)) {
                tryNextMethod();
            }
            break;
            
        case DapConnectionMethod::DNS_RESOLUTION:
            m_currentMethod = DapConnectionMethod::HTTP_PING;
            if (!httpPingTest(m_host, m_port)) {
                tryNextMethod();
            }
            break;
            
        case DapConnectionMethod::HTTP_PING:
            m_currentMethod = DapConnectionMethod::SOCKET_PING;
            if (!socketPingTest(m_host, m_port)) {
                // All methods failed
                DapConnectivityResult result(false, -1, DapConnectionMethod::SOCKET_PING, 
                                           "All connectivity check methods failed");
                completeCheck(result);
            }
            break;
            
        case DapConnectionMethod::SOCKET_PING:
        default:
            // All methods failed
            DapConnectivityResult result(false, -1, DapConnectionMethod::SOCKET_PING, 
                                       "All connectivity check methods failed");
            completeCheck(result);
            break;
    }
}

void DapServerConnectivityChecker::completeCheck(const DapConnectivityResult& result)
{
    m_lastResult = result;
    
    debugLog(QString("Connectivity check completed: %1, time: %2ms, method: %3")
             .arg(result.available ? "SUCCESS" : "FAILED")
             .arg(result.responseTime)
             .arg(static_cast<int>(result.method)));
    
    cleanup();
    
    if (result.available) {
        emit connectivityChecked(m_host, m_port, result.available, 
                               result.responseTime, result.method);
    } else {
        emit checkFailed(m_host, m_port, result.errorMessage);
    }
}

void DapServerConnectivityChecker::cleanup()
{
    // Clean up TCP connect test
    if (m_tcpSocket) {
        m_tcpSocket->deleteLater();
        m_tcpSocket = nullptr;
    }
    if (m_tcpTimer) {
        m_tcpTimer->deleteLater();
        m_tcpTimer = nullptr;
    }
    
    // Clean up DNS resolution test
    if (m_dnsLookupId != -1) {
        QHostInfo::abortHostLookup(m_dnsLookupId);
        m_dnsLookupId = -1;
    }
    
    // Clean up HTTP ping test
    if (m_httpPing) {
        m_httpPing->deleteLater();
        m_httpPing = nullptr;
    }
    
    // Clean up socket ping test
    if (m_socketPingSocket) {
        m_socketPingSocket->deleteLater();
        m_socketPingSocket = nullptr;
    }
    if (m_socketPingTimer) {
        m_socketPingTimer->deleteLater();
        m_socketPingTimer = nullptr;
    }
}

void DapServerConnectivityChecker::debugLog(const QString& message)
{
    if (m_debugMode) {
        qDebug() << "[DapServerConnectivityChecker]" << message;
    }
} 