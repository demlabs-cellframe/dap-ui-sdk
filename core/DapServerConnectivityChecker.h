#ifndef DAPSERVERCONNECTIVITYCHECKER_H
#define DAPSERVERCONNECTIVITYCHECKER_H

#include <QObject>
#include <QString>
#include <QElapsedTimer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QTimer>
#include <QNetworkReply>

class DapHttpPing;
class DapNetworkReply;

/**
 * @brief Connection check method types
 */
enum class DapConnectionMethod : int {
    TCP_CONNECT = 0,     // TCP connect test
    DNS_RESOLUTION = 1,  // DNS resolution
    HTTP_PING = 2,       // HTTP ping (traditional)
    SOCKET_PING = 3      // Socket ping (low-level)
};

/**
 * @brief Connectivity check result structure
 */
struct DapConnectivityResult {
    bool available = false;
    int responseTime = -1;
    DapConnectionMethod method = DapConnectionMethod::TCP_CONNECT;
    QString errorMessage;
    
    DapConnectivityResult() = default;
    DapConnectivityResult(bool avail, int time, DapConnectionMethod meth, const QString& error = "") 
        : available(avail), responseTime(time), method(meth), errorMessage(error) {}
};

/**
 * @brief Multi-level server availability checking system
 * 
 * This class is designed to solve server availability checking problems
 * in restricted networks (hotels, corporate networks with captive portal).
 * 
 * Uses a sequence of checks:
 * 1. TCP Connect Test - fast check for TCP connection establishment capability
 * 2. DNS Resolution Test - check server name resolution
 * 3. HTTP Ping Test - traditional HTTP check
 * 4. Socket Ping Test - low-level socket check
 * 
 * @author DapChainVPN Team
 * @date 2025-01-15
 */
class DapServerConnectivityChecker : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent object
     */
    explicit DapServerConnectivityChecker(QObject *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~DapServerConnectivityChecker();

    /**
     * @brief Start server availability check
     * @param host Server address
     * @param port Server port
     * @param timeoutMs Timeout for each check in milliseconds (default 2000)
     */
    void checkServerAvailability(const QString& host, quint16 port, int timeoutMs = 2000);

    /**
     * @brief Get host
     * @return Current host
     */
    QString getHost() const { return m_host; }

    /**
     * @brief Get port
     * @return Current port
     */
    quint16 getPort() const { return m_port; }

    /**
     * @brief Get last connectivity check result
     * @return Last result structure
     */
    DapConnectivityResult getLastResult() const { return m_lastResult; }

    /**
     * @brief Enable/disable debug mode
     * @param enabled Debug mode flag
     */
    void setDebugMode(bool enabled) { m_debugMode = enabled; }

signals:
    /**
     * @brief Signal emitted when connectivity check is completed
     * @param host Server host
     * @param port Server port
     * @param available Server availability flag
     * @param responseTime Response time in milliseconds
     * @param method Used check method
     */
    void connectivityChecked(const QString& host, quint16 port, bool available, 
                           int responseTime, DapConnectionMethod method);

    /**
     * @brief Signal emitted when check fails
     * @param host Server host
     * @param port Server port
     * @param errorMessage Error description
     */
    void checkFailed(const QString& host, quint16 port, const QString& errorMessage);

private slots:
    /**
     * @brief TCP connection success handler
     */
    void onTcpConnected();

    /**
     * @brief TCP connection error handler
     * @param error Socket error type
     */
    void onTcpError(QAbstractSocket::SocketError error);

    /**
     * @brief TCP connection timeout handler
     */
    void onTcpTimeout();

    /**
     * @brief DNS resolution completion handler
     * @param hostInfo DNS resolution result
     */
    void onDnsResolved(const QHostInfo& hostInfo);

    /**
     * @brief HTTP ping success handler
     * @param responseTime Response time
     */
    void onHttpPingSuccess(qint64 responseTime);

    /**
     * @brief HTTP ping error handler
     * @param errorMessage Error description
     */
    void onHttpPingError(const QString errorMessage);

private:
    /**
     * @brief Perform TCP connect test
     * @param host Server host
     * @param port Server port
     * @return Success flag
     */
    bool tcpConnectTest(const QString& host, quint16 port);

    /**
     * @brief Perform DNS resolution test
     * @param host Server host
     * @return Success flag
     */
    bool dnsResolutionTest(const QString& host);

    /**
     * @brief Perform HTTP ping test
     * @param host Server host
     * @param port Server port
     * @return Success flag
     */
    bool httpPingTest(const QString& host, quint16 port);

    /**
     * @brief Perform socket ping test
     * @param host Server host
     * @param port Server port
     * @return Success flag
     */
    bool socketPingTest(const QString& host, quint16 port);

    /**
     * @brief Try next check method
     */
    void tryNextMethod();

    /**
     * @brief Complete check with result
     * @param result Check result
     */
    void completeCheck(const DapConnectivityResult& result);

    /**
     * @brief Clean up resources
     */
    void cleanup();

    /**
     * @brief Output debug message
     * @param message Debug message
     */
    void debugLog(const QString& message);

private:
    QString m_host;
    quint16 m_port;
    int m_timeoutMs;
    bool m_debugMode;
    
    DapConnectionMethod m_currentMethod;
    QElapsedTimer m_timer;
    DapConnectivityResult m_lastResult;
    
    // TCP connect test
    QTcpSocket* m_tcpSocket;
    QTimer* m_tcpTimer;
    
    // DNS resolution test
    int m_dnsLookupId;
    
    // HTTP ping test
    DapHttpPing* m_httpPing;
    
    // Socket ping test
    QTcpSocket* m_socketPingSocket;
    QTimer* m_socketPingTimer;
};

#endif // DAPSERVERCONNECTIVITYCHECKER_H 